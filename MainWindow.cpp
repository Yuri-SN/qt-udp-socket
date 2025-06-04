#include "./MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);

    m_udpDateWorker = new UdpDateWorker(this);
    m_udpStringWorker = new UdpStringWorker(this);

    m_udpDateWorker->initSocket(12345);
    m_udpStringWorker->initSocket(23456);

    connect(m_udpDateWorker, &UdpDateWorker::sigSendTimeToGUI, this, &MainWindow::displayTime);
    connect(m_udpStringWorker, &UdpStringWorker::sigSendDataToGUI, this, &MainWindow::displayUserData);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [&] {
        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        m_udpDateWorker->sendDatagram(dataToSend);

        m_timer->start(TIMER_DELAY);
    });

    connect(m_ui->startButton, &QPushButton::clicked, this, &MainWindow::onButtonStartClicked);
    connect(m_ui->stopButton, &QPushButton::clicked, this, &MainWindow::onButtonStopClicked);
    connect(m_ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
}

MainWindow::~MainWindow() {
    delete m_ui;
}

void MainWindow::onButtonStartClicked() {
    m_timer->start(TIMER_DELAY);
}

// Slots

void MainWindow::onButtonStopClicked() {
    m_timer->stop();
}

void MainWindow::onSendButtonClicked() {
    QByteArray byteData = m_ui->userInput->text().toUtf8();

    m_udpStringWorker->sendDatagram(byteData);
}

void MainWindow::displayTime(QDateTime data) {
    countPkg();

    auto text =
        QString("Текущее время: %1. Принято пакетов: %2").arg(data.toString(), QString::number(m_counterPck));

    m_ui->te_result->append(text);
}

void MainWindow::displayUserData(QString from, QByteArray data) {
    countPkg();

    auto text = QString("Принято сообщение от: %1, размер сообщения(байт): %2").arg(from).arg(data.size());

    m_ui->te_result->append(text);
}

// Private

void MainWindow::countPkg() {
    m_counterPck++;
    if (m_counterPck % 20 == 0) {
        m_ui->te_result->clear();
    }
}
