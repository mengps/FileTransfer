#include "discoverconnection.h"

#include <QNetworkDatagram>

DiscoverConnection::DiscoverConnection(QObject *parent)
    : QUdpSocket (parent)
{
    bind(QHostAddress::Any, 43801);
    connect(this, &QUdpSocket::readyRead, this, &DiscoverConnection::processDatagram);
}

DiscoverConnection *DiscoverConnection::instance()
{
    static DiscoverConnection d;
    return &d;
}

DiscoverConnection::~DiscoverConnection()
{

}

QString DiscoverConnection::getName(const QHostAddress &address) const
{
    return m_accessPoints.key(address);
}

QHostAddress DiscoverConnection::getAddress(const QString &name) const
{
    return m_accessPoints[name];
}

QString DiscoverConnection::name() const
{
    return m_name;
}

void DiscoverConnection::setName(const QString &name)
{
    if (name != m_name)
    {
        m_name = name;
        emit nameChanged();
    }
}

void DiscoverConnection::discover()
{
    m_accessPoints.clear();
    writeDatagram("[DISCOVER]", QHostAddress::Broadcast, 43801);
}

void DiscoverConnection::connectToName(const QString &name)
{
    writeDatagram("[CONNECT]##" + m_name.toLocal8Bit(), getAddress(name), 43801);
}

void DiscoverConnection::processDatagram()
{
    while (hasPendingDatagrams())
    {
        QNetworkDatagram datagram = receiveDatagram();
        if (!datagram.senderAddress().isNull() && datagram.senderPort() != -1)
        {
            if (datagram.data() == "[DISCOVER]")
            {
                writeDatagram("[NAME]##" + m_name.toLocal8Bit(), datagram.senderAddress(),
                              quint16(datagram.senderPort()));
            }
            else if (datagram.data().left(8) == "[NAME]##")
            {
                QString name = QString::fromLocal8Bit(datagram.data().mid(8));
                m_accessPoints[name] = datagram.senderAddress();
                qDebug() << name << datagram.senderAddress();
                emit newAccessPoint(name);
            }
            else if (datagram.data().left(11) == "[CONNECT]##")
            {
                QString name = QString::fromLocal8Bit(datagram.data().mid(11));
                emit newConnection(name);
            }
        }
    }
}
