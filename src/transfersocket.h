#ifndef TRANSFERSOCKET_H
#define TRANSFERSOCKET_H

#include <QTcpSocket>

class TransferSocketPrivate;
class TransferSocket : public QTcpSocket
{
    Q_OBJECT

public:
    TransferSocket();
    ~TransferSocket();

    void requestNewConnection();

    Q_INVOKABLE void setDestAddress(const QHostAddress &address);
    Q_INVOKABLE void sendFile(const QUrl &url);
    Q_INVOKABLE void writeToSocket(const QByteArray &data) { QTcpSocket::write(data); }

signals:
    void hasError(const QString &error);

public slots:
    void processRecvBlock();

private:
    TransferSocketPrivate *d;
};

#endif // TRANSFERSOCKET_H
