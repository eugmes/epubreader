/* Copyright © 2010 Євгеній Мещеряков <eugen@debian.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainwindowbase.h"

class MainWindow : public MainWindowBase {
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE openFile NOTIFY fileNameChanged)
    Q_PROPERTY(bool fullSize READ fullSize WRITE setFullSize NOTIFY fullSizeChanged)
public:
    explicit MainWindow(QWidget *parent = 0);
    QString fileName() const;
    bool fullSize() const;

    void setFirstWindow();

public Q_SLOTS:
    void openFile(QString newFileName);
    void chooseFile();
    void setFullSize(bool set);
    void showLibrary();
    void showToc(const QByteArray &tocDocument);

Q_SIGNALS:
    void fileNameChanged();
    void fullSizeChanged();
    void newWindow();
    void openTocDocumentRequest(const QString &path);

private Q_SLOTS:
    void showSettingsDialog();

protected:
    virtual void updateWindowOrientation();
    virtual void showEvent(QShowEvent *event);

private:
    QString m_fileName;
    bool m_orientationOverride;
    bool m_showLibrary;

    void showHelperWindow(QMainWindow *win);
};

#endif
