#include "./UdpDateWorker.h"

UdpDateWorker::UdpDateWorker(QObject* parent) : QObject(parent) {}

/*!
 * @brief Метод инициализирует UDP сервер
 */
void UdpDateWorker::initSocket(quint16 port) {
    m_port = port;
    m_udpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    m_udpSocket->bind(QHostAddress::LocalHost, m_port);

    connect(m_udpSocket, &QUdpSocket::readyRead, this, &UdpDateWorker::readPendingDatagrams);
}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UdpDateWorker::readDatagram(QNetworkDatagram datagram) {
    QByteArray data;
    data = datagram.data();

    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sigSendTimeToGUI(dateTime);
}

/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UdpDateWorker::sendDatagram(QByteArray data) {
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    m_udpSocket->writeDatagram(data, QHostAddress::LocalHost, m_port);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UdpDateWorker::readPendingDatagrams(void) {
    /*
     *  Производим чтение принятых датаграмм
     */
    while (m_udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
        readDatagram(datagram);
    }
}
