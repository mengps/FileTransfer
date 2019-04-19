#ifndef DISCOVERCONNECTION_H
#define DISCOVERCONNECTION_H

#include <QUdpSocket>

class DiscoverConnection : public QUdpSocket
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:   
    static DiscoverConnection* instance();
    ~DiscoverConnection();

    QString getName(const QHostAddress &address) const;
    QHostAddress getAddress(const QString &name) const;

    QString name() const;
    void setName(const QString &name);

    Q_INVOKABLE void discover();

signals:
    void nameChanged();
    void newAccessPoint(const QString &name);

private:
    DiscoverConnection(QObject *parent = nullptr);
    void processDatagram();

    QString m_name = "未命名";
    QMap<QString, QHostAddress> m_accessPoints;
};

#endif // DISCOVERCONNECTION_H
