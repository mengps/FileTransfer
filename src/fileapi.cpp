#include "fileapi.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QQmlFile>

FileApi::FileApi(QObject *parent)
    : QObject(parent)
{

}

QString FileApi::fileToIcon(const QUrl &url)
{
    QString fileName = QQmlFile::urlToLocalFileOrQrc(url);
    QFileIconProvider provider;
    QFileInfo info = QFileInfo(fileName);
    QIcon icon = provider.icon(info);
    QPixmap image = QPixmap(icon.pixmap(32, 32));
    QDir dir;
    QString iconDir = qApp->applicationDirPath() + "/FileIcon/";
    if (!dir.exists(iconDir))
        dir.mkdir(iconDir);
    image.save(iconDir + info.fileName() + "_icon.png", "PNG");

    return "file:///" + iconDir + info.fileName() + "_icon.png";
}

QString FileApi::fileName(const QUrl &url)
{
    QString fileName = QQmlFile::urlToLocalFileOrQrc(url);
    return QFileInfo(fileName).fileName();
}

QString FileApi::convertByte(int byte)
{
    if (byte > (1024 * 1024 * 1024))
        return QString::asprintf("%.2fGB", byte / (1024.0 * 1024.0 * 1024.0));
    else if (byte > (1024 * 1024))
        return QString::asprintf("%.2fMB", byte / (1024.0 * 1024.0));
    else if (byte > (1024))
        return QString::asprintf("%.2fKB", byte / (1024.0));
    else
        return QString::asprintf("%d bytes", int(byte));
}
