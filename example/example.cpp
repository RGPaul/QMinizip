/*
--------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 Ralph-Gordon Paul. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
--------------------------------------------------------------------------------
*/

#include <QDebug>
#include <QStandardPaths>

#include <QMinizip.h>

void unzipExample();
void zipExample();

int main(int argc, char *argv[])
{
    Q_UNUSED(argc)
    Q_UNUSED(argv)

    unzipExample();
    zipExample();

    return EXIT_SUCCESS;
}

void unzipExample()
{
    QString downloadPath =
            QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    // our Minizip class
    rgp::QMinizip zipper;

    // we assume that we have a test.zip file in our downloads directory
    // and that the zip file is password protected

    qDebug() << "opening zip file: " << downloadPath << "/test.zip";

    if (zipper.openUnzipFile(downloadPath + "/test.zip", "test"))
        qDebug() << "opened zip file";
    else
        qDebug() << "openening zip file failed";

    if (zipper.unzipFiles(downloadPath + "/test"))
        qDebug() << "unzip sucessful";
    else
        qDebug() << "unzip failed";

    zipper.closeUnzipFile();
}

void zipExample()
{
    QString downloadPath =
            QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    // our Minizip class
    rgp::QMinizip zipper;

    // we assume that we have 3 images ( 1.jpg, 2.jpg and 3.jpg ) in our
    // downloads folder

    QString filename = downloadPath + "/images.zip";

    QStringList files = { "1.jpg", "2.jpg", "3.jpg" };

    qDebug() << "opening zip file: " << filename;
    if (zipper.createZipFile(filename))
        qDebug() << "opened zip file";
    else
        qDebug() << "openening zip file failed";

    for (QString file : files) {
        qDebug() << "adding file: " << file;

        if (zipper.addFileToZip(downloadPath + '/' + file, file)) {
            qDebug() << "added " << file << " successfully";
        } else {
            qDebug() << "failed to add " << file;
        }
    }

    zipper.closeZipFile();
}
