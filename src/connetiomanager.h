#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QTcpServer>

class ConnectionManager : public QTcpServer
{
    Q_OBJECT

public:
    ConnectionManager(QObject *parent = nullptr);
    ~ConnectionManager();

signals:
    void hasNewConnection(QTcpSocket *socket);

protected:
    virtual void incomingConnection(qintptr handle);
};

#endif // CONNECTIONMANAGER_H
