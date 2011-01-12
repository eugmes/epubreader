/****************************************************************************
**
** This file was modified by Eugeniy Meshcheryakov <eugen@debian.org> for use
** in epubreader
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QtGlobal>
#include "zipreader.h"
#include <QDateTime>
#include <qplatformdefs.h>
#include <QtEndian>
#include <QDebug>
#include <QDir>

#include <zlib.h>

#if 0
#define ZDEBUG qDebug
#else
#define ZDEBUG if (0) qDebug
#endif

static inline uint readUInt(const uchar *data)
{
    return (data[0]) + (data[1]<<8) + (data[2]<<16) + (data[3]<<24);
}

static inline ushort readUShort(const uchar *data)
{
    return (data[0]) + (data[1]<<8);
}

static int inflate(Bytef *dest, ulong *destLen, const Bytef *source, ulong sourceLen)
{
    z_stream stream;
    int err;

    stream.next_in = (Bytef*)source;
    stream.avail_in = (uInt)sourceLen;
    if ((uLong)stream.avail_in != sourceLen)
        return Z_BUF_ERROR;

    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen)
        return Z_BUF_ERROR;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;

    err = inflateInit2(&stream, -MAX_WBITS);
    if (err != Z_OK)
        return err;

    err = inflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        inflateEnd(&stream);
        if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
            return Z_DATA_ERROR;
        return err;
    }
    *destLen = stream.total_out;

    err = inflateEnd(&stream);
    return err;
}

static QFile::Permissions modeToPermissions(quint32 mode)
{
    QFile::Permissions ret;
    if (mode & S_IRUSR)
        ret |= QFile::ReadOwner;
    if (mode & S_IWUSR)
        ret |= QFile::WriteOwner;
    if (mode & S_IXUSR)
        ret |= QFile::ExeOwner;
    if (mode & S_IRUSR)
        ret |= QFile::ReadUser;
    if (mode & S_IWUSR)
        ret |= QFile::WriteUser;
    if (mode & S_IXUSR)
        ret |= QFile::ExeUser;
    if (mode & S_IRGRP)
        ret |= QFile::ReadGroup;
    if (mode & S_IWGRP)
        ret |= QFile::WriteGroup;
    if (mode & S_IXGRP)
        ret |= QFile::ExeGroup;
    if (mode & S_IROTH)
        ret |= QFile::ReadOther;
    if (mode & S_IWOTH)
        ret |= QFile::WriteOther;
    if (mode & S_IXOTH)
        ret |= QFile::ExeOther;
    return ret;
}

static QDateTime readMSDosDate(const uchar *src)
{
    uint dosDate = readUInt(src);
    quint64 uDate;
    uDate = (quint64)(dosDate >> 16);
    uint tm_mday = (uDate & 0x1f);
    uint tm_mon =  ((uDate & 0x1E0) >> 5);
    uint tm_year = (((uDate & 0x0FE00) >> 9) + 1980);
    uint tm_hour = ((dosDate & 0xF800) >> 11);
    uint tm_min =  ((dosDate & 0x7E0) >> 5);
    uint tm_sec =  ((dosDate & 0x1f) << 1);

    return QDateTime(QDate(tm_year, tm_mon, tm_mday), QTime(tm_hour, tm_min, tm_sec));
}

struct LocalFileHeader
{
    uchar signature[4]; //  0x04034b50
    uchar version_needed[2];
    uchar general_purpose_bits[2];
    uchar compression_method[2];
    uchar last_mod_file[4];
    uchar crc_32[4];
    uchar compressed_size[4];
    uchar uncompressed_size[4];
    uchar file_name_length[2];
    uchar extra_field_length[2];
};

struct DataDescriptor
{
    uchar crc_32[4];
    uchar compressed_size[4];
    uchar uncompressed_size[4];
};

struct CentralFileHeader
{
    uchar signature[4]; // 0x02014b50
    uchar version_made[2];
    uchar version_needed[2];
    uchar general_purpose_bits[2];
    uchar compression_method[2];
    uchar last_mod_file[4];
    uchar crc_32[4];
    uchar compressed_size[4];
    uchar uncompressed_size[4];
    uchar file_name_length[2];
    uchar extra_field_length[2];
    uchar file_comment_length[2];
    uchar disk_start[2];
    uchar internal_file_attributes[2];
    uchar external_file_attributes[4];
    uchar offset_local_header[4];
};

struct EndOfDirectory
{
    uchar signature[4]; // 0x06054b50
    uchar this_disk[2];
    uchar start_of_directory_disk[2];
    uchar num_dir_entries_this_disk[2];
    uchar num_dir_entries[2];
    uchar directory_size[4];
    uchar dir_start_offset[4];
    uchar comment_length[2];
};

struct FileHeader
{
    CentralFileHeader h;
    QByteArray file_name;
    QByteArray extra_field;
    QByteArray file_comment;
};

ZipReader::FileInfo::FileInfo()
    : isDir(false), isFile(false), isSymLink(false), crc32(0), size(0)
{
}

ZipReader::FileInfo::~FileInfo()
{
}

ZipReader::FileInfo::FileInfo(const FileInfo &other)
{
    operator=(other);
}

ZipReader::FileInfo& ZipReader::FileInfo::operator=(const FileInfo &other)
{
    filePath = other.filePath;
    isDir = other.isDir;
    isFile = other.isFile;
    isSymLink = other.isSymLink;
    permissions = other.permissions;
    crc32 = other.crc32;
    size = other.size;
    lastModified = other.lastModified;
    return *this;
}

bool ZipReader::FileInfo::isValid() const
{
    return isDir || isFile || isSymLink;
}

class ZipPrivate
{
public:
    ZipPrivate(QIODevice *device, bool ownDev)
        : device(device), ownDevice(ownDev), dirtyFileTree(true), start_of_directory(0)
    {
    }

    ~ZipPrivate()
    {
        if (ownDevice)
            delete device;
    }

    void fillFileInfo(int index, ZipReader::FileInfo &fileInfo) const;

    QIODevice *device;
    bool ownDevice;
    bool dirtyFileTree;
    QList<FileHeader> fileHeaders;
    QByteArray comment;
    uint start_of_directory;
};

void ZipPrivate::fillFileInfo(int index, ZipReader::FileInfo &fileInfo) const
{
    FileHeader header = fileHeaders.at(index);
    fileInfo.filePath = QString::fromLocal8Bit(header.file_name);
    const quint32 mode = (qFromLittleEndian<quint32>(&header.h.external_file_attributes[0]) >> 16) & 0xFFFF;
    fileInfo.isDir = S_ISDIR(mode);
    fileInfo.isFile = S_ISREG(mode);
    fileInfo.isSymLink = S_ISLNK(mode);
    fileInfo.permissions = modeToPermissions(mode);
    fileInfo.crc32 = readUInt(header.h.crc_32);
    fileInfo.size = readUInt(header.h.uncompressed_size);
    fileInfo.lastModified = readMSDosDate(header.h.last_mod_file);
}

class ZipReaderPrivate : public ZipPrivate
{
public:
    ZipReaderPrivate(QIODevice *device, bool ownDev)
        : ZipPrivate(device, ownDev), status(ZipReader::NoError)
    {
    }

    void scanFiles();

    ZipReader::Status status;
};

void ZipReaderPrivate::scanFiles()
{
    if (!dirtyFileTree)
        return;

    if (! (device->isOpen() || device->open(QIODevice::ReadOnly))) {
        status = ZipReader::FileOpenError;
        return;
    }

    if ((device->openMode() & QIODevice::ReadOnly) == 0) { // only read the index from readable files.
        status = ZipReader::FileReadError;
        return;
    }

    dirtyFileTree = false;
    uchar tmp[4];
    device->read((char *)tmp, 4);
    if (readUInt(tmp) != 0x04034b50) {
        qWarning() << "QZip: not a zip file!";
        return;
    }

    // find EndOfDirectory header
    int i = 0;
    int start_of_directory = -1;
    int num_dir_entries = 0;
    EndOfDirectory eod;
    while (start_of_directory == -1) {
        int pos = device->size() - sizeof(EndOfDirectory) - i;
        if (pos < 0 || i > 65535) {
            qWarning() << "QZip: EndOfDirectory not found";
            return;
        }

        device->seek(pos);
        device->read((char *)&eod, sizeof(EndOfDirectory));
        if (readUInt(eod.signature) == 0x06054b50)
            break;
        ++i;
    }

    // have the eod
    start_of_directory = readUInt(eod.dir_start_offset);
    num_dir_entries = readUShort(eod.num_dir_entries);
    ZDEBUG("start_of_directory at %d, num_dir_entries=%d", start_of_directory, num_dir_entries);
    int comment_length = readUShort(eod.comment_length);
    if (comment_length != i)
        qWarning() << "QZip: failed to parse zip file.";
    comment = device->read(qMin(comment_length, i));


    device->seek(start_of_directory);
    for (i = 0; i < num_dir_entries; ++i) {
        FileHeader header;
        int read = device->read((char *) &header.h, sizeof(CentralFileHeader));
        if (read < (int)sizeof(CentralFileHeader)) {
            qWarning() << "QZip: Failed to read complete header, index may be incomplete";
            break;
        }
        if (readUInt(header.h.signature) != 0x02014b50) {
            qWarning() << "QZip: invalid header signature, index may be incomplete";
            break;
        }

        int l = readUShort(header.h.file_name_length);
        header.file_name = device->read(l);
        if (header.file_name.length() != l) {
            qWarning() << "QZip: Failed to read filename from zip index, index may be incomplete";
            break;
        }
        l = readUShort(header.h.extra_field_length);
        header.extra_field = device->read(l);
        if (header.extra_field.length() != l) {
            qWarning() << "QZip: Failed to read extra field in zip file, skipping file, index may be incomplete";
            break;
        }
        l = readUShort(header.h.file_comment_length);
        header.file_comment = device->read(l);
        if (header.file_comment.length() != l) {
            qWarning() << "QZip: Failed to read read file comment, index may be incomplete";
            break;
        }

        ZDEBUG("found file '%s'", header.file_name.data());
        fileHeaders.append(header);
    }
}

//////////////////////////////  Reader

/*!
    \class QZipReader::FileInfo
    \internal
    Represents one entry in the zip table of contents.
*/

/*!
    \variable FileInfo::filePath
    The full filepath inside the archive.
*/

/*!
    \variable FileInfo::isDir
    A boolean type indicating if the entry is a directory.
*/

/*!
    \variable FileInfo::isFile
    A boolean type, if it is one this entry is a file.
*/

/*!
    \variable FileInfo::isSymLink
    A boolean type, if it is one this entry is symbolic link.
*/

/*!
    \variable FileInfo::permissions
    A list of flags for the permissions of this entry.
*/

/*!
    \variable FileInfo::crc32
    The calculated checksum as a crc32 type.
*/

/*!
    \variable FileInfo::size
    The total size of the unpacked content.
*/

/*!
    \variable FileInfo::d
    \internal
    private pointer.
*/

/*!
    \class QZipReader
    \internal
    \since 4.5

    \brief the QZipReader class provides a way to inspect the contents of a zip
    archive and extract individual files from it.

    QZipReader can be used to read a zip archive either from a file or from any
    device. An in-memory QBuffer for instance.  The reader can be used to read
    which files are in the archive using fileInfoList() and entryInfoAt() but
    also to extract individual files using fileData() or even to extract all
    files in the archive using extractAll()
*/

/*!
    Create a new zip archive that operates on the \a fileName.  The file will be
    opened with the \a mode.
*/
ZipReader::ZipReader(const QString &archive, QIODevice::OpenMode mode)
{
    QScopedPointer<QFile> f(new QFile(archive));
    f->open(mode);
    ZipReader::Status status;
    if (f->error() == QFile::NoError)
        status = NoError;
    else {
        if (f->error() == QFile::ReadError)
            status = FileReadError;
        else if (f->error() == QFile::OpenError)
            status = FileOpenError;
        else if (f->error() == QFile::PermissionsError)
            status = FilePermissionsError;
        else
            status = FileError;
    }

    d = new ZipReaderPrivate(f.data(), /*ownDevice=*/true);
    f.take();
    d->status = status;
}

/*!
    Create a new zip archive that operates on the archive found in \a device.
    You have to open the device previous to calling the constructor and only a
    device that is readable will be scanned for zip filecontent.
 */
ZipReader::ZipReader(QIODevice *device)
    : d(new ZipReaderPrivate(device, /*ownDevice=*/false))
{
    Q_ASSERT(device);
}

/*!
    Desctructor
*/
ZipReader::~ZipReader()
{
    close();
    delete d;
}

/*!
    Returns device used for reading zip archive.
*/
QIODevice* ZipReader::device() const
{
    return d->device;
}

/*!
    Returns true if the user can read the file; otherwise returns false.
*/
bool ZipReader::isReadable() const
{
    return d->device->isReadable();
}

/*!
    Returns true if the file exists; otherwise returns false.
*/
bool ZipReader::exists() const
{
    QFile *f = qobject_cast<QFile*> (d->device);
    if (f == 0)
        return true;
    return f->exists();
}

/*!
    Returns the list of files the archive contains.
*/
QList<ZipReader::FileInfo> ZipReader::fileInfoList() const
{
    d->scanFiles();
    QList<ZipReader::FileInfo> files;
    for (int i = 0; i < d->fileHeaders.size(); ++i) {
        ZipReader::FileInfo fi;
        d->fillFileInfo(i, fi);
        files.append(fi);
    }
    return files;

}

/*!
    Fetch the file contents from the zip archive and return the uncompressed bytes.
*/
QByteArray ZipReader::fileData(const QString &fileName) const
{
    d->scanFiles();
    int i;
    for (i = 0; i < d->fileHeaders.size(); ++i) {
        if (QString::fromLocal8Bit(d->fileHeaders.at(i).file_name) == fileName)
            break;
    }
    if (i == d->fileHeaders.size())
        return QByteArray();

    FileHeader header = d->fileHeaders.at(i);

    int compressed_size = readUInt(header.h.compressed_size);
    int uncompressed_size = readUInt(header.h.uncompressed_size);
    int start = readUInt(header.h.offset_local_header);
    //qDebug("uncompressing file %d: local header at %d", i, start);

    d->device->seek(start);
    LocalFileHeader lh;
    d->device->read((char *)&lh, sizeof(LocalFileHeader));
    uint skip = readUShort(lh.file_name_length) + readUShort(lh.extra_field_length);
    d->device->seek(d->device->pos() + skip);

    int compression_method = readUShort(lh.compression_method);
    //qDebug("file=%s: compressed_size=%d, uncompressed_size=%d", fileName.toLocal8Bit().data(), compressed_size, uncompressed_size);

    //qDebug("file at %lld", d->device->pos());
    QByteArray compressed = d->device->read(compressed_size);
    if (compression_method == 0) {
        // no compression
        compressed.truncate(uncompressed_size);
        return compressed;
    } else if (compression_method == 8) {
        // Deflate
        //qDebug("compressed=%d", compressed.size());
        compressed.truncate(compressed_size);
        QByteArray baunzip;
        ulong len = qMax(uncompressed_size,  1);
        int res;
        do {
            baunzip.resize(len);
            res = inflate((uchar*)baunzip.data(), &len,
                          (uchar*)compressed.constData(), compressed_size);

            switch (res) {
            case Z_OK:
                if ((int)len != baunzip.size())
                    baunzip.resize(len);
                break;
            case Z_MEM_ERROR:
                qWarning("QZip: Z_MEM_ERROR: Not enough memory");
                break;
            case Z_BUF_ERROR:
                len *= 2;
                break;
            case Z_DATA_ERROR:
                qWarning("QZip: Z_DATA_ERROR: Input data is corrupted");
                break;
            }
        } while (res == Z_BUF_ERROR);
        return baunzip;
    }
    qWarning() << "QZip: Unknown compression method";
    return QByteArray();
}

/*!
    \enum QZipReader::Status

    The following status values are possible:

    \value NoError  No error occurred.
    \value FileReadError    An error occurred when reading from the file.
    \value FileOpenError    The file could not be opened.
    \value FilePermissionsError The file could not be accessed.
    \value FileError        Another file error occurred.
*/

/*!
    Returns a status code indicating the first error that was met by QZipReader,
    or QZipReader::NoError if no error occurred.
*/
ZipReader::Status ZipReader::status() const
{
    return d->status;
}

/*!
    Close the zip file.
*/
void ZipReader::close()
{
    d->device->close();
}
