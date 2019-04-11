#include "fileblock.h"
#include "filesender.h"
#include "filemanager.h"

#include <QtConcurrent>
#include <QFile>
#include <QFileInfo>
#include <QHostAddress>
#include <QQmlFile>

const int maxBlockSize = 1024;

FileSender::FileSender(QObject *parent)
    : QTcpSocket (parent)
{

}

FileSender::~FileSender()
{

}

void FileSender::sendFile(const QUrl &url)
{
    if (state() != SocketState::ConnectedState)
        requestNewConnection();

    QtConcurrent::run([this, url]()
    {
        QTime time;
        time.start();
        QFile file(QQmlFile::urlToLocalFileOrQrc(url));
        file.open(QIODevice::ReadOnly);

        qint32 offset = 0;
        qint32 totalSize = qint32(file.size());
        QString fileName = QFileInfo(QQmlFile::urlToLocalFileOrQrc(url)).fileName();
        while (offset < totalSize)
        {
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
            if (time.elapsed() >= 1000 || offset >= totalSize)
            {
                time.restart();
                QMetaObject::invokeMethod(FileManager::instance(), "updateWriteFile",
                                          Q_ARG(QString, fileName), Q_ARG(int, offset));
            }
        }

        file.close();
    });
}

void FileSender::requestNewConnection()
{
    connectToHost(QHostAddress::LocalHost, 43800);
    waitForConnected(5000);
}
