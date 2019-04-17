#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QObject>

class ConnectionManager;
class FileSender;
class FileTransfer : public QObject
{
    Q_OBJECT

public:
    static FileTransfer* instance();
    ~FileTransfer();

    Q_INVOKABLE void setAccessPoint(const QString &name);
    Q_INVOKABLE void sendFile(const QUrl &url);

signals:
    void error(const QString &error);

private:
    FileTransfer(QObject *parent = nullptr);
    FileSender *m_sender;
    ConnectionManager *m_connection;
};

#endif // FILETRANSFER_H
