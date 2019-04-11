#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QObject>

class ConnectionManager;
class FileSender;
class FileTransfer : public QObject
{
    Q_OBJECT

public:
    FileTransfer(QObject *parent = nullptr);
    ~FileTransfer();

    Q_INVOKABLE void sendFile(const QUrl &url);

private:
    FileSender *m_sender;
    ConnectionManager *m_connection;
};

#endif // FILETRANSFER_H
