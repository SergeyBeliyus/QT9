#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{

}


// инициализация UDP сервера
void UDPworker::InitSocket(int bindPort)
{
    bindPort_ = bindPort;
    serviceUdpSocket = new QUdpSocket(this);
    serviceUdpSocket->bind(QHostAddress::LocalHost, bindPort_);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);

}

// Обработка Датаграммы
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();

    QDataStream inStr(&data, QIODevice::ReadOnly);
    if (bindPort_ == 12345)
    {
        QDateTime dateTime;
        inStr >> dateTime;
        emit sig_sendTimeToGUI(dateTime);
    }
    else if(bindPort_ == 123456)
    {
        QString dateInputText;
        inStr >> dateInputText;
        QString localAddress = serviceUdpSocket->localAddress().toString();
        emit sig_sendInputTextToGUI(dateInputText, localAddress, bindPort_);
    }
}

// Передача Датаграммы
void UDPworker::SendDatagram(QByteArray data)
{
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, bindPort_);
}

// Чтение датаграммы из сокета
void UDPworker::readPendingDatagrams( void )
{
    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}
