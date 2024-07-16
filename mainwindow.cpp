#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket(12345);

    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);

    udpWorkerSendDatagram = new UDPworker(this);
    udpWorkerSendDatagram->InitSocket(123456);
    connect(udpWorkerSendDatagram, &UDPworker::sig_sendInputTextToGUI, this, &MainWindow::DisplayInputText);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}


void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                                                                "Принято пакетов " + QString::number(counterPck));


}

void MainWindow::DisplayInputText(QString data, QString address, int port)
{

    ui->te_result->append("Принято сообщение от " + address + ":" +
                          QString::number(port) + ", размер сообщения в байтах: " +
                          QString::number(data.size()));
}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}

void MainWindow::on_pb_sendDatagram_clicked()
{

    QString dateInputText = ui->le_inputText->text();

    QByteArray dataToSend;
    QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

    outStr << dateInputText;

    udpWorkerSendDatagram->SendDatagram(dataToSend);
}

