#pragma once

#include <QDateTime>
#include <QNetworkDatagram>
#include <QUdpSocket>

class UdpDateWorker : public QObject {
    Q_OBJECT

  public:
    explicit UdpDateWorker(QObject* parent = nullptr);

    void initSocket(quint16 port);
    void readDatagram(QNetworkDatagram datagram);
    void sendDatagram(QByteArray data);

  private slots:
    void readPendingDatagrams(void);

  private:
    QUdpSocket* m_udpSocket;
    quint16 m_port;

  signals:
    void sigSendTimeToGUI(QDateTime data);
};
