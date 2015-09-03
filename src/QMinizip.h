#include <QObject>
#include <QFile>

namespace rgp {

class QMinizip : QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList *unzippedFiles READ unzippedFiles)
    Q_PROPERTY(QStringList *zipFileContents READ zipFileContents)
public:
    explicit QMinizip(QObject *parent = 0);
    ~QMinizip();
    
    // accessors
    QStringList *unzippedFiles() const;
    QStringList *zipFileContents() const;
    
    // zip
    Q_INVOKABLE bool createZipFile(QString *file, QString *password = nullptr);
    Q_INVOKABLE bool addFileToZip(QString *file, QString *newname);
    Q_INVOKABLE bool closeZipFile();

    // unzip
    Q_INVOKABLE bool openUnzipFile(QString *file, QString *password = nullptr);
    Q_INVOKABLE bool unzipFilesTo(QString *path, bool overwrite = false);
    Q_INVOKABLE bool closeUnzipFile();

signals:
    void error(QString message);
    
public slots:
    
private:
    QFile *m_zipFile { nullptr };
    QFile *m_unzipFile { nullptr };
    QString *m_password { nullptr };
    
    QStringList *m_unzippedFiles { nullptr };
};
} // namespace rgp
