#include "discoverconnection.h"
#include "fileblock.h"
#include "filemanager.h"
#include "connectionmanager.h"
#include "transfersocket.h"

#include <QApplication>
#include <QDir>
#include <QTime>
#include <QThread>
#include <QTcpSocket>

ConnectionManager::ConnectionManager(QObject *parent)
    : QTcpServer (parent)
{
    listen(QHostAddress::Any, 43800);
}

ConnectionManager::~ConnectionManager()
{

}

void ConnectionManager::incomingConnection(qintptr handle)
{
    QThread *thread = new QThread;
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    TransferSocket *socket = new TransferSocket;
    if (!socket->setSocketDescriptor(handle)) {
        qDebug() << "Socket Error: " << socket->errorString();
    } else {
        emit hasNewConnection(socket);
        qDebug() << "Connected Socket: " << handle;
    }
    socket->moveToThread(thread);
    thread->start();
}
