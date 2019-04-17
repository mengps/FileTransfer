#ifndef FILESENDER_H
#define FILESENDER_H

#include "discoverconnection.h"
#include <QTcpSocket>

class FileSender : public QTcpSocket
{
    Q_OBJECT

public:
    FileSender(QObject *parent = nullptr);
    ~FileSender();

    void requestNewConnection();
    void setAccessPoint(const AccessPoint &ap);

    Q_INVOKABLE void sendFile(const QUrl &url);
    Q_INVOKABLE void writeToSocket(const QByteArray &data) { QTcpSocket::write(data); }


signals:
    void hasError(const QString &error);

private:
    AccessPoint m_accessPoint;
};

#endif // FILESENDER_H
