#pragma once

#include <QMainWindow>
#include <QTimer>

#include "./UdpDateWorker.h"
#include "./UdpStringWorker.h"

#define TIMER_DELAY 1000

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  private slots:
    void onButtonStartClicked();
    void onButtonStopClicked();
    void onSendButtonClicked();
    void displayTime(QDateTime data);
    void displayUserData(QString from, QByteArray data);

  private:
    Ui::MainWindow* m_ui;
    QTimer* m_timer;
    UdpDateWorker* m_udpDateWorker;
    UdpStringWorker* m_udpStringWorker;
    uint32_t m_counterPck = 0;

    void countPkg();
};
