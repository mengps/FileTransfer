#include "fileblock.h"
#include "filemanager.h"
#include "transfersocket.h"

#include <QtConcurrent>
#include <QFile>
#include <QFileInfo>
#include <QHostAddress>
#include <QQmlFile>

static const int maxBlockSize = 1024;

struct File
{
    QFile *file;
    qint32 size;
};

class TransferSocketPrivate
{
public:
    int m_maxTaskNum = 8;
    QString m_cachePath;
    QByteArray m_recvData;
    QMap<QString, File> m_recvFiles;
    QHostAddress m_destAddress;
};

TransferSocket::TransferSocket()
{
    d = new TransferSocketPrivate;
    d->m_cachePath = qApp->applicationDirPath() + "/FileRecv/";
    QDir dir;
    if (!dir.exists(d->m_cachePath)) {
        dir.mkpath(d->m_cachePath);
    }

    connect(this, &QTcpSocket::readyRead, this, [this]() {
        d->m_recvData += readAll();
        processRecvBlock();
    });
}

TransferSocket::~TransferSocket()
{
    delete d;
}

void TransferSocket::requestNewConnection()
{
    abort();
    connectToHost(d->m_destAddress, 43800);
    waitForConnected(5000);
}

void TransferSocket::setDestAddress(const QHostAddress &address)
{
    if (d->m_destAddress != address)
        d->m_destAddress = address;
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
    if (d->m_recvData.size() > 0) {
        FileBlock block;
        QDataStream in(&d->m_recvData, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_12);
        in >> block;

        if (block.isEmpty())
            return;

        QString fileName = QString::fromLocal8Bit(block.fileName);

        if (!d->m_recvFiles[fileName].file) {
            QFile *file = new QFile(d->m_cachePath + fileName);
            file->open(QIODevice::WriteOnly);
            d->m_recvFiles[fileName].file = file;
            d->m_recvFiles[fileName].size = 0;
            QMetaObject::invokeMethod(FileManager::instance(), "addReadFile",
                                      Q_ARG(QString, fileName), Q_ARG(int, block.fileSize));
            QThread::msleep(100);
        }

        if (d->m_recvFiles[fileName].size < block.fileSize) {
            d->m_recvFiles[fileName].size += block.blockSize;
            d->m_recvFiles[fileName].file->write(block.dataBlock);
            qDebug() << block;
        }

        if (d->m_recvFiles[fileName].size == block.fileSize) {
            d->m_recvFiles[fileName].file->close();
            d->m_recvFiles[fileName].file->deleteLater();
            d->m_recvFiles.remove(fileName);
            QMetaObject::invokeMethod(FileManager::instance(), "updateReadFile",
                                      Q_ARG(QString, fileName), Q_ARG(int, block.fileSize));
        }

        if (time.elapsed() >= 1000) {
            time.restart();
            QMetaObject::invokeMethod(FileManager::instance(), "updateReadFile",
                                      Q_ARG(QString, fileName), Q_ARG(int, d->m_recvFiles[fileName].size));
        }

        d->m_recvData.remove(0, block.size());
        if (d->m_recvData.size() > 0)  //如果还有则继续处理
            processRecvBlock();
    }
}
