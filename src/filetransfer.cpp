#include "discoverconnection.h"
#include "connetiomanager.h"
#include "filemanager.h"
#include "filetransfer.h"
#include "transfersocket.h"

#include <QFileInfo>
#include <QQmlFile>
#include <QThread>

FileTransfer::FileTransfer(QObject *parent)
    : QObject (parent)
{
    m_connection = new ConnectionManager(this);
    connect(m_connection, &ConnectionManager::hasNewConnection, this, [this](QTcpSocket *socket)
    {
        if (m_socket)
            m_socket->deleteLater();
        m_socket = qobject_cast<TransferSocket *>(socket);
    });
}

FileTransfer *FileTransfer::instance()
{
    static FileTransfer fileTransfer;
    return &fileTransfer;
}

FileTransfer::~FileTransfer()
{

}

void FileTransfer::setAccessPoint(const QString &name)
{
    DiscoverConnection *dc = DiscoverConnection::instance();
    QHostAddress address = dc->getAddress(name);

    QThread *thread = new QThread;
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    TransferSocket *socket = new TransferSocket;
    socket->moveToThread(thread);
    thread->start();
    m_socket = socket;
    QMetaObject::invokeMethod(m_socket, "setDestAddress", Q_ARG(QHostAddress, address));
}

void FileTransfer::sendFile(const QUrl &url)
{
    QFileInfo info(QQmlFile::urlToLocalFileOrQrc(url));
    FileManager::instance()->addWriteFile(info.fileName(), int(info.size()));
    QMetaObject::invokeMethod(m_socket, "sendFile", Q_ARG(QUrl, url));
}
