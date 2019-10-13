#include "fileblock.h"
#include "filemanager.h"
#include "transfersocket.h"

#include <QtConcurrent>
#include <QFile>
#include <QFileInfo>
#include <QHostAddress>
#include <QQmlFile>

const int maxBlockSize = 1024;

TransferSocket::TransferSocket()
{
    m_cachePath = qApp->applicationDirPath() + "/FileRecv/";
    QDir dir;
    if (!dir.exists(m_cachePath)) {
        dir.mkpath(m_cachePath);
    }

    connect(this, &QTcpSocket::readyRead, this, [this]() {
        m_recvData += readAll();
        processRecvBlock();
    });
}

TransferSocket::~TransferSocket()
{

}

void TransferSocket::requestNewConnection()
{
    abort();
    connectToHost(m_destAddress, 43800);
    waitForConnected(5000);
}

void TransferSocket::setDestAddress(const QHostAddress &address)
{
    if (m_destAddress != address)
        m_destAddress = address;
    requestNewConnection();
}

void TransferSocket::sendFile(const QUrl &url)
{
    if (state() != SocketState::ConnectedState)
        requestNewConnection();

    QtConcurrent::run([this, url]() {
        QTime time;
        time.start();
        QFile file(QQmlFile::urlToLocalFileOrQrc(url));
        file.open(QIODevice::ReadOnly);

        qint32 offset = 0;
        qint32 totalSize = qint32(file.size());
        QString fileName = QFileInfo(QQmlFile::urlToLocalFileOrQrc(url)).fileName();
        while (offset < totalSize) {
            file.seek(offset);
            QByteArray dataBlock = file.read(maxBlockSize);
            FileBlock block = { qint16(dataBlock.size()), offset, totalSize,
                                fileName.toLocal8Bit(), dataBlock};
            QByteArray data;
            QDataStream out(&data, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_12);
            out << block;
            QMetaObject::invokeMethod(this, "writeToSocket", Q_ARG(QByteArray, data));

            offset += dataBlock.size();
            if (time.elapsed() >= 1000 || offset >= totalSize) {
                time.restart();
                QMetaObject::invokeMethod(FileManager::instance(), "updateWriteFile",
                                          Q_ARG(QString, fileName), Q_ARG(int, offset));
            }
        }

        file.close();
    });
}

void TransferSocket::processRecvBlock()
{
    static QTime time = QTime::currentTime();
    if (m_recvData.size() > 0) {
        FileBlock block;
        QDataStream in(&m_recvData, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_12);
        in >> block;

        if (block.isEmpty())
            return;

        QString fileName = QString::fromLocal8Bit(block.fileName);

        if (!m_recvFiles[fileName]) {
            QFile *file = new QFile(m_cachePath + fileName);
            file->open(QIODevice::WriteOnly);
            m_recvFiles[fileName] = file;
            m_recvFileSize[fileName] = 0;
            QMetaObject::invokeMethod(FileManager::instance(), "addReadFile",
                                      Q_ARG(QString, fileName), Q_ARG(int, block.fileSize));
            QThread::msleep(100);
        }

        if (m_recvFileSize[fileName] < block.fileSize) {
            m_recvFileSize[fileName] += block.blockSize;
            m_recvFiles[fileName]->write(block.dataBlock);
            qDebug() << block;
        }

        if (m_recvFileSize[fileName] == block.fileSize) {
            m_recvFiles[fileName]->close();
            m_recvFiles[fileName]->deleteLater();
            m_recvFiles.remove(fileName);
            m_recvFileSize.remove(fileName);
            QMetaObject::invokeMethod(FileManager::instance(), "updateReadFile",
                                      Q_ARG(QString, fileName), Q_ARG(int, block.fileSize));
        }

        if (time.elapsed() >= 1000) {
            time.restart();
            QMetaObject::invokeMethod(FileManager::instance(), "updateReadFile",
                                      Q_ARG(QString, fileName), Q_ARG(int, m_recvFileSize[fileName]));
        }

        m_recvData.remove(0, block.size());
        if (m_recvData.size() > 0)  //如果还有则继续处理
            processRecvBlock();
    }
}
