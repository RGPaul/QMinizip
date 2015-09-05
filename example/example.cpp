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

    // path to a test zip
    QString filename = downloadPath + "/test.zip";
    // password for the test zip
    QString pwd = "test";
    // target folder
    QString targetpath = downloadPath + "/test";

    qDebug() << "opening zip file: " << filename;
    if (zipper.openUnzipFile(&filename, &pwd))
        qDebug() << "opened zip file";
    else
        qDebug() << "openening zip file failed";

    if (zipper.unzipFiles(&targetpath))
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

    QString filename = downloadPath + "/images.zip";

    qDebug() << "opening zip file: " << filename;
    if (zipper.createZipFile(&filename))
        qDebug() << "opened zip file";
    else
        qDebug() << "openening zip file failed";

    QString file1 = downloadPath + "/1.jpg";
    QString file2 = downloadPath + "/2.jpg";
    QString file3 = downloadPath + "/3.jpg";

    QString name1 = "1.jpg";
    QString name2 = "subfolder/2.jpg";
    QString name3 = "3.jpg";

    if (zipper.addFileToZip(&file1, &name1)) {
        qDebug() << "added file successfully";
    } else {
        qDebug() << "failed to add file";
    }

    if (zipper.addFileToZip(&file2, &name2)) {
        qDebug() << "added file successfully";
    } else {
        qDebug() << "failed to add file";
    }

    if (zipper.addFileToZip(&file3, &name3)) {
        qDebug() << "added file successfully";
    } else {
        qDebug() << "failed to add file";
    }

    zipper.closeZipFile();
}
