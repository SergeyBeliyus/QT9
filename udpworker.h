#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( int bind_port);
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );


private slots:
    void readPendingDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;
    int bindPort_;

signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_sendInputTextToGUI(QString data, QString address, int port);

};

#endif // UDPWORKER_H
