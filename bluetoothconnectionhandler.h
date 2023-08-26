#ifndef BLUETOOTHCONNECTIONHANDLER_H
#define BLUETOOTHCONNECTIONHANDLER_H

#include <QObject>
#include <QtBluetooth>
#include <QDebug>

class BluetoothConnectionHandler : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothConnectionHandler(QObject *parent = nullptr);

    void startDeviceDiscovery();
    void connectToDevice(const QBluetoothDeviceInfo &deviceInfo, const QString &serviceUuid);
    void sendCommand(const QByteArray &command);

signals:
    void dataReceived(const QByteArray &data);
    void boardReset();

private slots:
    void onDeviceDiscovered(const QBluetoothDeviceInfo &deviceInfo);
    void onDeviceDiscoveryFinished();

    void onServiceDiscovered(const QBluetoothServiceInfo &serviceInfo);
    void onServiceDiscoveryFinished();

    void onConnected();
    void onDisconnected();
    void onReadyRead();

private:
    QBluetoothDeviceDiscoveryAgent *deviceDiscoveryAgent;
    QBluetoothServiceDiscoveryAgent *serviceDiscoveryAgent;
    QBluetoothSocket *socket;

    QString deviceAddress;
    QString deviceName;

    QByteArray data;
};

#endif // BLUETOOTHCONNECTIONHANDLER_H
