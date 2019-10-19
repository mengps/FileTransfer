#include "discoverconnection.h"
#include "framelesswindow.h"
#include "fileapi.h"
#include "filemanager.h"
#include "filetransfer.h"
#include "scanneritem.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("FileTransfer");
    QCoreApplication::setOrganizationDomain("mps.filetransfer.app");

    qRegisterMetaType<FileInfo>("FileInfo");
    qRegisterMetaType<QHostAddress>("QHostAddress");
    qmlRegisterType<FramelessWindow>("an.window", 1, 0, "FramelessWindow");
    qmlRegisterType<FileInfo>("an.file", 1, 0, "FileInfo");
    qmlRegisterType<ScannerItem>("an.item", 1, 0, "ScannerItem");

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
