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

#include "QMinizip.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

#include <zlib.h>
#include <zconf.h>
#include "minizip/zip.h"
#include "minizip/unzip.h"

using namespace rgp;

QMinizip::QMinizip(QObject *parent) : QObject(parent) {}

QMinizip::~QMinizip()
{
    // close any open files
    closeZipFile();
    closeUnzipFile();

    // free memory
    if (m_unzippedFiles != nullptr) delete m_unzippedFiles;
}

// -----------------------------------------------------------------------------
// accessors
// -----------------------------------------------------------------------------

QStringList * QMinizip::unzippedFiles() const
{
    return m_unzippedFiles;
}

// -----------------------------------------------------------------------------
// zip functions
// -----------------------------------------------------------------------------

bool QMinizip::createZipFile(QString filepath, bool append, QString password)
{
    m_password = password;
    int appendData = (append ? APPEND_STATUS_ADDINZIP : APPEND_STATUS_CREATE);

    m_zipFile = zipOpen(filepath.toUtf8().data(), appendData);

    if (!m_zipFile)
        return false;

    return true;
}

bool QMinizip::addFileToZip(QString filepath, QString newname)
{
    QFile file = { filepath };

    if (!file.exists())
        return false;

    if (!file.open(QIODevice::ReadOnly))
        return false;

    QFileInfo fileInfo = QFileInfo(file);

    QByteArray data = file.readAll();
    QDateTime created = fileInfo.created();

    return addDataToZip(&data, newname, created);
}

bool QMinizip::addDataToZip(QByteArray *data, QString newname,
                            QDateTime created)
{
    if (data == nullptr || newname == nullptr || m_zipFile == nullptr)
        return false;

    if (!created.isValid())
        created = QDateTime::currentDateTime();

    zip_fileinfo zipInfo = {{0,0,0,0,0,0},0,0,0};

    zipInfo.tmz_date.tm_sec = created.time().second();
    zipInfo.tmz_date.tm_min = created.time().minute();
    zipInfo.tmz_date.tm_hour = created.time().hour();
    zipInfo.tmz_date.tm_mday = created.date().day();
    zipInfo.tmz_date.tm_mon = created.date().month() - 1; // 0-11 vs 1-12
    zipInfo.tmz_date.tm_year = created.date().year();

    if (m_password.isEmpty())
    {
        if (zipOpenNewFileInZip(m_zipFile, // file
                                  newname.toUtf8().data(), // filename
                                  &zipInfo, // zipfi
                                  NULL, // extrafield_local
                                  0, // size_extrafield_local
                                  NULL, // extrafield_global
                                  0, // size_extrafield_global
                                  NULL, //comment
                                  Z_DEFLATED, // method
                                  Z_DEFAULT_COMPRESSION // level
                                  ) != Z_OK) {
            return false;
        }
    }

    // TODO: add password usage

    if (zipWriteInFileInZip(m_zipFile, data->constData(), data->size()) != Z_OK)
        return false;

    if (zipCloseFileInZip(m_zipFile) != Z_OK)
        return false;

    return true;
}

bool QMinizip::closeZipFile()
{
    m_password = nullptr;
    if (m_zipFile == nullptr)
        return false;

    if (zipClose(m_zipFile, NULL) != Z_OK) {
        m_zipFile = nullptr;
        return false;
    }

    m_zipFile = nullptr;
    return true;
}

// -----------------------------------------------------------------------------
// unzip functions
// -----------------------------------------------------------------------------

bool QMinizip::openUnzipFile(QString filepath, QString password)
{
    m_password = password;

    if (m_unzippedFiles != nullptr)
        delete m_unzippedFiles;

    m_unzippedFiles = new QStringList();

    m_unzipFile = unzOpen(filepath.toUtf8().data());

    return m_unzipFile != nullptr;
}

bool QMinizip::unzipFiles(QString targetpath, bool overwrite)
{
    if (targetpath.isEmpty() || m_unzipFile == nullptr)
        return false;

    // targetpath needs a trailing /
    if (!targetpath.endsWith('/'))
        targetpath.append('/');

    bool success = true;

    char buffer[4096] = {0};

    const char *password = { nullptr };

    if (!m_password.isEmpty())
        password = m_password.toStdString().c_str();

    if (unzGoToFirstFile(m_unzipFile) != UNZ_OK)
        return false;

    do // we iterate over all files
    {
        // open current file
        if (password != nullptr) {
            // we have a password - so we need to open with it
            if (unzOpenCurrentFilePassword(m_unzipFile, password) != UNZ_OK) {
                success = false;
                break;
            }
        } else {
            // we don't have a password - just open
            if (unzOpenCurrentFile(m_unzipFile) != UNZ_OK) {
                success = false;
                break;
            }
        }

        // reading file info
        unz_file_info fileInfo;
        if (unzGetCurrentFileInfo(m_unzipFile, &fileInfo, NULL, 0, NULL, 0,
                                    NULL, 0) != UNZ_OK) {
            success = false;
            unzCloseCurrentFile(m_unzipFile);
            break;
        }

        // get filename from the fileInfo
        char cfilename[fileInfo.size_filename + 1];
        unzGetCurrentFileInfo(m_unzipFile, &fileInfo, cfilename,
                              fileInfo.size_filename + 1, NULL, 0, NULL, 0);
        cfilename[fileInfo.size_filename] = '\0';

        // check filename for paths
        QString filename = { cfilename };

        // replace all '\' with '/'
        int index = -1;
        while ((index = filename.indexOf('\\')) != -1) {
            filename.replace(index, 1, '/');
        }
        QString fullpath = targetpath + filename;

        // create directory / parent directories
        index = fullpath.lastIndexOf('/');
        if (index != -1) {
            QDir path = { fullpath.left(index) };
            path.mkpath(".");
        }

        QFile *file = nullptr;
        int read = 0;
        while ((read = unzReadCurrentFile(m_unzipFile, buffer, 4096)) > 0)
        {
            if (file == nullptr)
            {
                if (QFile::exists(fullpath) && !overwrite)
                    break;

                file = new QFile(fullpath);

                if (!file->open(QIODevice::WriteOnly)) {
                    delete file;
                    file = nullptr;
                    break;
                }
            }
            QByteArray data = QByteArray::fromRawData(buffer, read);
            file->write(data);
        }

        // check if we got an error
        if (read < 0)
            success = false;

        if (file != nullptr) {
            file->close(); // flush the data to the file
            delete file;
            file = nullptr;
        }

        // we are done with the file -> close the file
        if (unzCloseCurrentFile(m_unzipFile) != UNZ_OK) {
            success = false;
            break;
        }

    } // goto next file
    while (unzGoToNextFile(m_unzipFile) == UNZ_OK && success);

    return success;
}

bool QMinizip::closeUnzipFile()
{
    m_password = nullptr;
    if (m_unzipFile == nullptr)
        return false;

    if (unzClose(m_unzipFile) != UNZ_OK) {
        m_unzipFile = nullptr;
        return false;
    }

    m_unzipFile = nullptr;
    return true;
}
