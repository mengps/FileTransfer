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
    m_sender->moveToThread(thread);
    thread->start();
}

FileTransfer::~FileTransfer()
{

}

void FileTransfer::sendFile(const QUrl &url)
{
    QFileInfo info(QQmlFile::urlToLocalFileOrQrc(url));
    FileManager::instance()->addWriteFile(info.fileName(), int(info.size()));
    QMetaObject::invokeMethod(m_sender, "sendFile", Q_ARG(QUrl, url));
}
