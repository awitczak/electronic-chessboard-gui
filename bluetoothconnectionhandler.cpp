#include "bluetoothconnectionhandler.h"

BluetoothConnectionHandler::BluetoothConnectionHandler(QObject *parent)
    : QObject{parent}
{
    deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    connect(deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BluetoothConnectionHandler::onDeviceDiscovered);
    connect(deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BluetoothConnectionHandler::onDeviceDiscoveryFinished);

    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);

    connect(socket, &QBluetoothSocket::connected,
            this, &BluetoothConnectionHandler::onConnected);
    connect(socket, &QBluetoothSocket::disconnected,
            this, &BluetoothConnectionHandler::onDisconnected);
    connect(socket, &QBluetoothSocket::readyRead,
            this, &BluetoothConnectionHandler::onReadyRead);

    serviceDiscoveryAgent = new QBluetoothServiceDiscoveryAgent(this);

    connect(serviceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
            this, &BluetoothConnectionHandler::onServiceDiscovered);
    connect(serviceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::finished,
            this, &BluetoothConnectionHandler::onServiceDiscoveryFinished);
}

void BluetoothConnectionHandler::startDeviceDiscovery()
{
    deviceDiscoveryAgent->start();
}

void BluetoothConnectionHandler::connectToDevice(const QBluetoothDeviceInfo &deviceInfo, const QString &serviceUuid)
{
    socket->connectToService(deviceInfo.address(), QBluetoothUuid(serviceUuid));
}

void BluetoothConnectionHandler::onDeviceDiscovered(const QBluetoothDeviceInfo &deviceInfo)
{
    qDebug() << "Discovered device: " << deviceInfo.name() << "Address: " << deviceInfo.address().toString();

    if (deviceInfo.name() == "BT_CHESSBOARD") {
        deviceName = deviceInfo.name();
        deviceAddress = deviceInfo.address().toString();

        connectToDevice(deviceInfo, "00001101-0000-1000-8000-00805F9B34FB");

//        serviceDiscoveryAgent->start();
    }
}

void BluetoothConnectionHandler::onServiceDiscovered(const QBluetoothServiceInfo &serviceInfo)
{
    qDebug() << "Found service with name:" << serviceInfo.serviceName();
}

void BluetoothConnectionHandler::onServiceDiscoveryFinished()
{
    serviceDiscoveryAgent->stop();
}

void BluetoothConnectionHandler::onDeviceDiscoveryFinished()
{
    deviceDiscoveryAgent->stop();
}

void BluetoothConnectionHandler::onReadyRead()
{
    QByteArray data = socket->readAll();

    qDebug() << "Data from BluetoothDevice: " << data;

    emit dataReceived(data);
}

void BluetoothConnectionHandler::onConnected()
{
    qDebug() << "Connected to Bluetooth device!";
}

void BluetoothConnectionHandler::onDisconnected()
{
    qDebug() << "Disconnected from Bluetooth device!";
}
