#include "discoverconnection.h"
#include "fileblock.h"
#include "filemanager.h"
#include "filereceiver.h"

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
    FileReceiver *receiver = new FileReceiver;
    if (!receiver->setSocketDescriptor(handle))
        qDebug() << "Socket Error: " << receiver->errorString();
    else qDebug() << "Connected Socket: " << handle;
    receiver->moveToThread(thread);
    thread->start();
}

FileReceiver::FileReceiver(QObject *parent)
    : QTcpSocket (parent)
{
    m_cachePath = qApp->applicationDirPath() + "/FileRecv/";
    QDir dir;
    if (!dir.exists(m_cachePath))
    {
        dir.mkpath(m_cachePath);
    }

    connect(this, &QTcpSocket::readyRead, this, [this]()
    {
        m_recvData += readAll();
        processRecvBlock();
    });
}

FileReceiver::~FileReceiver()
{

}

void FileReceiver::processRecvBlock()
{  
    static QTime time = QTime::currentTime();
    if (m_recvData.size() > 0)
    {
        FileBlock block;
        QDataStream in(&m_recvData, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_12);
        in >> block;

        if (block.isEmpty())
            return;

        QString fileName = QString::fromLocal8Bit(block.fileName);

        if (!m_recvFiles[fileName])
        {
            QFile *file = new QFile(m_cachePath + fileName);
            file->open(QIODevice::WriteOnly);
            m_recvFiles[fileName] = file;
            m_recvFileSize[fileName] = 0;
            QMetaObject::invokeMethod(FileManager::instance(), "addReadFile",
                                      Q_ARG(QString, fileName), Q_ARG(int, block.fileSize));
            QThread::msleep(100);
        }

        if (m_recvFileSize[fileName] < block.fileSize)
        {
            m_recvFileSize[fileName] += block.blockSize;
            m_recvFiles[fileName]->write(block.dataBlock);
            qDebug() << block;
        }

        if (m_recvFileSize[fileName] == block.fileSize)
        {
            m_recvFiles[fileName]->close();
            m_recvFiles[fileName]->deleteLater();
            m_recvFiles.remove(fileName);
            m_recvFileSize.remove(fileName);
            QMetaObject::invokeMethod(FileManager::instance(), "updateReadFile",
                                      Q_ARG(QString, fileName), Q_ARG(int, block.fileSize));
        }

        if (time.elapsed() >= 1000)
        {
            time.restart();
            QMetaObject::invokeMethod(FileManager::instance(), "updateReadFile",
                                      Q_ARG(QString, fileName), Q_ARG(int, m_recvFileSize[fileName]));
        }

        m_recvData.remove(0, block.size());
        if (m_recvData.size() > 0)  //如果还有则继续处理
            processRecvBlock();
    }
}
