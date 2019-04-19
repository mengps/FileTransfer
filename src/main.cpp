#include "discoverconnection.h"
#include "fileapi.h"
#include "filemanager.h"
#include "filetransfer.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qRegisterMetaType<FileInfo>("FileInfo");
    qRegisterMetaType<QHostAddress>("QHostAddress");
    qmlRegisterType<FileInfo>("an.file", 1, 0, "FileInfo");

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("fileApi", new FileApi);
    engine.rootContext()->setContextProperty("fileManager", FileManager::instance());
    engine.rootContext()->setContextProperty("fileTransfer", FileTransfer::instance());
    engine.rootContext()->setContextProperty("discoverCon", DiscoverConnection::instance());
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
