#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QQmlListProperty>

class FileInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(int offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(int fileSize READ fileSize WRITE setFileSize NOTIFY fileSizeChanged)

public:
    FileInfo(QObject *parent = nullptr);
    FileInfo(const FileInfo &other, QObject *parent = nullptr);
    FileInfo(const QString &fileName, int offset, int fileSize, QObject *parent = nullptr);
    ~FileInfo();

    QString fileName() const;
    void setFileName(const QString &fileName);

    int offset() const;
    void setOffset(int offset);

    int fileSize() const;
    void setFileSize(int fileSize);

signals:
    void fileNameChanged();
    void offsetChanged();
    void fileSizeChanged();

private:
    QString m_fileName;
    int m_offset;
    int m_fileSize;
};

class FileManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<FileInfo> readFiles READ readFiles NOTIFY readFilesChanged)
    Q_PROPERTY(QQmlListProperty<FileInfo> writeFiles READ writeFiles NOTIFY writeFilesChanged)

public:
    static FileManager* instance();
    ~FileManager();

    QQmlListProperty<FileInfo> readFiles();
    QQmlListProperty<FileInfo> writeFiles();

public slots:
    void addReadFile(const QString &fileName, qint32 totalSize);
    void updateReadFile(const QString &fileName, qint32 offset);

    void addWriteFile(const QString &fileName, qint32 totalSize);
    void updateWriteFile(const QString &fileName, qint32 offset);

signals:
    void readFilesChanged();
    void writeFilesChanged();
    void readFileComplete(const QString &fileName);
    void writeFileComplete(const QString &fileName);

private:
    FileManager(QObject *parent = nullptr);

    QList<FileInfo *> m_readFiles;
    QQmlListProperty<FileInfo> *m_readFilesProxy;
    QList<FileInfo *> m_writeFiles;
    QQmlListProperty<FileInfo> *m_writeFilesProxy;
};

#endif // FILEMANAGER_H
