#include "./UdpStringWorker.h"

UdpStringWorker::UdpStringWorker(QObject* parent) : QObject(parent) {}

/*!
 * @brief Метод инициализирует UDP сервер
 */
void UdpStringWorker::initSocket(quint16 port) {
    m_port = port;
    m_udpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    m_udpSocket->bind(QHostAddress::LocalHost, m_port);

    connect(m_udpSocket, &QUdpSocket::readyRead, this, &UdpStringWorker::readPendingDatagrams);
}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UdpStringWorker::readDatagram(QNetworkDatagram datagram) {
    auto data = datagram.data();
    auto from = datagram.senderAddress().toString();

    emit sigSendDataToGUI(from, data);
}

/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UdpStringWorker::sendDatagram(QByteArray data) {
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    m_udpSocket->writeDatagram(data, QHostAddress::LocalHost, m_port);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UdpStringWorker::readPendingDatagrams(void) {
    /*
     *  Производим чтение принятых датаграмм
     */
    while (m_udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_udpSocket->receiveDatagram();
        readDatagram(datagram);
    }
}
