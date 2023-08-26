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
    socket->abort();
    socket->connectToService(deviceInfo.address(), QBluetoothUuid(serviceUuid));
}

void BluetoothConnectionHandler::sendCommand(const QByteArray &command)
{
    if (socket->state() == QBluetoothSocket::SocketState::ConnectedState) {
        socket->write(command);
    } else {
        qDebug() << "Not connected to a Bluetooth device. Cannot send command.";
    }

    qDebug() << command;
}

void BluetoothConnectionHandler::onDeviceDiscovered(const QBluetoothDeviceInfo &deviceInfo)
{
    qDebug() << "Discovered device: " << deviceInfo.name() << "Address: " << deviceInfo.address().toString();

    if (deviceInfo.name() == "BT_CHESSBOARD") {

        deviceDiscoveryAgent->stop();

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
    data += socket->readAll();

    while (data.contains('\n')) {
        int newlineIdx = data.indexOf('\n');
        QByteArray completeLine = data.left(newlineIdx - 1);
        data.remove(0, newlineIdx + 1);

        emit dataReceived(completeLine);
    }
}

void BluetoothConnectionHandler::onConnected()
{
    qDebug() << "Connected to Bluetooth device!";

    sendCommand("RESET");

    emit boardReset();
}

void BluetoothConnectionHandler::onDisconnected()
{
    qDebug() << "Disconnected from Bluetooth device!";
}
