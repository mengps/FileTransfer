#include "discoverconnection.h"
#include "filesender.h"
#include "filereceiver.h"
#include "filemanager.h"
#include "filetransfer.h"

#include <QFileInfo>
#include <QQmlFile>
#include <QThread>

FileTransfer::FileTransfer(QObject *parent)
    : QObject (parent)
{
    m_connection = new ConnectionManager(this);

    QThread *thread = new QThread;
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    m_sender = new FileSender;
    connect(m_sender, &FileSender::hasError, this, &FileTransfer::error);
    m_sender->moveToThread(thread);
    thread->start();
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
    m_sender->setAccessPoint(dc->getAccessPoint(name));
}

void FileTransfer::sendFile(const QUrl &url)
{
    QFileInfo info(QQmlFile::urlToLocalFileOrQrc(url));
    FileManager::instance()->addWriteFile(info.fileName(), int(info.size()));
    QMetaObject::invokeMethod(m_sender, "sendFile", Q_ARG(QUrl, url));
}
