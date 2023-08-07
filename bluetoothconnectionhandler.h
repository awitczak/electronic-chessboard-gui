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

signals:
    void dataReceived(const QByteArray &data);

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
};

#endif // BLUETOOTHCONNECTIONHANDLER_H
