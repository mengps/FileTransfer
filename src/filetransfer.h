#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QObject>

class ConnectionManager;
class TransferSocket;
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
    TransferSocket *m_socket;
    ConnectionManager *m_connection;
};

#endif // FILETRANSFER_H
