#pragma once

#include <QDateTime>
#include <QNetworkDatagram>
#include <QUdpSocket>

class UdpStringWorker : public QObject {
    Q_OBJECT

  public:
    explicit UdpStringWorker(QObject* parent = nullptr);

    void initSocket(quint16 port);
    void readDatagram(QNetworkDatagram datagram);
    void sendDatagram(QByteArray data);

  private slots:
    void readPendingDatagrams(void);

  private:
    QUdpSocket* m_udpSocket;
    quint16 m_port;

  signals:
    void sigSendDataToGUI(QString from, QByteArray data);
};
