#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include <QTcpServer>
#include <QTcpSocket>

class ConnectionManager : public QTcpServer
{
public:
    ConnectionManager(QObject *parent = nullptr);
    ~ConnectionManager();

protected:
    virtual void incomingConnection(qintptr handle);
};

class QFile;
class FileReceiver : public QTcpSocket
{
    Q_OBJECT

public:
    FileReceiver(QObject *parent = nullptr);
    ~FileReceiver();

public slots:
    void processRecvBlock();

private:
    int m_maxRecvNum = 8;
    QString m_cachePath;
    QByteArray m_recvData;
    QMap<QString, QFile *> m_recvFiles;
    QMap<QString, qint32> m_recvFileSize;
};

#endif // FILERECEIVER_H
