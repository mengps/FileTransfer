#ifndef TRANSFERSOCKET_H
#define TRANSFERSOCKET_H

#include <QHostAddress>
#include <QTcpSocket>

class QFile;
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
    int m_maxRecvNum = 8;
    QString m_cachePath;
    QByteArray m_recvData;
    //可以用一个struct File { QFile *file; qint32 size; }
    QMap<QString, QFile *> m_recvFiles;
    QMap<QString, qint32> m_recvFileSize;
    QHostAddress m_destAddress;
};

#endif // TRANSFERSOCKET_H
