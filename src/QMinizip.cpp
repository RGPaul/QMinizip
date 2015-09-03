#include "QMinizip.h"

using namespace rgp;

explicit QMinizip::QMinizip(QObject *parent) : QObject(parent) {}
QMinizip::~QMinizip() {}

// -----------------------------------------------------------------------------
// accessors
// -----------------------------------------------------------------------------

QStringList * QMinizip::unzippedFiles() const
{

}

QStringList * QMinizip::zipFileContents() const
{

}

// -----------------------------------------------------------------------------
// zip functions
// -----------------------------------------------------------------------------

bool QMinizip::createZipFile(QString *file, QString *password)
{

}

bool QMinizip::addFileToZip(QString *file, QString *newname)
{

}

bool QMinizip::closeZipFile()
{

}

// -----------------------------------------------------------------------------
// unzip functions
// -----------------------------------------------------------------------------

bool QMinizip::openUnzipFile(QString *file, QString *password)
{

}

bool QMinizip::unzipFilesTo(QString *path, bool overwrite)
{

}

bool QMinizip::closeUnzipFile()
{

}
