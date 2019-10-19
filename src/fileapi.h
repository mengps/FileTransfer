#ifndef FILEAPI_H
#define FILEAPI_H

#include <QObject>
#include <QUrl>

class FileApi : public QObject
{
    Q_OBJECT

public:
    FileApi(QObject *parent = nullptr);

    Q_INVOKABLE QString fileToIcon(const QUrl &url);
    Q_INVOKABLE QString fileName(const QUrl &url);
    Q_INVOKABLE QString convertByte(int byte);
};

#endif // FILEAPI_H
