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

#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE openFile NOTIFY fileNameChanged)
    Q_PROPERTY(bool fullSize READ fullSize WRITE setFullSize NOTIFY fullSizeChanged)
    Q_PROPERTY(qreal textSizeMultiplier READ textSizeMultiplier WRITE setTextSizeMultiplier NOTIFY textSizeMultiplierChanged)
public:
    explicit MainWindow(QWidget *parent = 0);
    QString fileName() const;
    bool fullSize() const;

    qreal textSizeMultiplier() const;
    void setTextSizeMultiplier(qreal factor);

public slots:
    void openFile(QString newFileName);
    void chooseFile();
    void setFullSize(bool set);
    void showLibrary();

signals:
    void fileNameChanged();
    void fullSizeChanged();
    void newWindow();
    void textSizeMultiplierChanged();

private slots:
    void showSettingsDialog();

private:
    QString m_fileName;
    qreal m_textSizeMultiplier;
};

#endif
