#ifndef FILESENDER_H
#define FILESENDER_H

#include <QTcpSocket>

class FileSender : public QTcpSocket
{
    Q_OBJECT

public:
    FileSender(QObject *parent = nullptr);
    ~FileSender();

    Q_INVOKABLE void sendFile(const QUrl &url);
    Q_INVOKABLE void writeToSocket(const QByteArray &data) { QTcpSocket::write(data); }
    void requestNewConnection();
};

#endif // FILESENDER_H
