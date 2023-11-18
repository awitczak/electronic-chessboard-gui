#include "serialporthandler.h"

static const char blankString[] = QT_TRANSLATE_NOOP("", "N/A");
static constexpr std::chrono::seconds kWriteTimeout = std::chrono::seconds{5};

SerialPortHandler::SerialPortHandler(QWidget *parent)
    : QWidget(parent),
    m_timer(new QTimer(this)),
    m_serialPort(new QSerialPort(this))
{
    // ui
    serialPort = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(serialPort);

    btn_apply = new QPushButton();
    cb_serialPortInfo = new QComboBox();
    cb_baudRate = new QComboBox();
    cb_dataBits = new QComboBox();
    cb_parityBits = new QComboBox();
    cb_stopBits = new QComboBox();
    cb_flowControl = new QComboBox();
    chb_localEchoEnabled = new QCheckBox();

    output_serialPort = new QPlainTextEdit();
    input_serialPort = new InputTextBox();
    btn_send = new QPushButton();
    btn_open = new QPushButton();
    btn_close = new QPushButton();

    input_serialPort->setMaximumHeight(40);
    input_serialPort->setStyleSheet("background-color: 255, 255, 255;");
    btn_apply->setText("Apply");
    btn_send->setText("Send");
    btn_open->setText("Open");
    btn_close->setText("Close");

    output_serialPort->setStyleSheet("background-color: 255, 255, 255;");

    QWidget *firstRow = new QWidget();
    QWidget *secondRow = new QWidget();
    QWidget *thirdRow = new QWidget();

    firstRow->setMaximumHeight(50);
    secondRow->setMaximumHeight(50);

    QHBoxLayout *layout_firstRow = new QHBoxLayout(firstRow);
    layout_firstRow->addWidget(btn_apply);
    layout_firstRow->addWidget(cb_serialPortInfo);
    layout_firstRow->addWidget(cb_baudRate);
    layout_firstRow->addWidget(cb_dataBits);
    layout_firstRow->addWidget(cb_parityBits);
    layout_firstRow->addWidget(cb_stopBits);
    layout_firstRow->addWidget(cb_flowControl);
    layout_firstRow->addWidget(chb_localEchoEnabled);

    QHBoxLayout *layout_secondRow = new QHBoxLayout(secondRow);
    layout_secondRow->addWidget(btn_open);
    layout_secondRow->addWidget(btn_close);
    layout_secondRow->addWidget(input_serialPort);
    layout_secondRow->addWidget(btn_send);

    QHBoxLayout *layout_thirdRow = new QHBoxLayout(thirdRow);
    layout_thirdRow->addWidget(output_serialPort);

    layout->addWidget(firstRow);
    layout->addWidget(secondRow);
    layout->addWidget(thirdRow);

    connect(btn_apply, &QPushButton::clicked, this, &SerialPortHandler::apply);
    connect(cb_serialPortInfo, &QComboBox::currentIndexChanged, this, &SerialPortHandler::showPortInfo);
    connect(cb_serialPortInfo, &QComboBox::currentIndexChanged, this, &SerialPortHandler::checkCustomDevicePathPolicy);
    connect(cb_baudRate, &QComboBox::currentIndexChanged, this, &SerialPortHandler::checkCustomBaudRatePolicy);

    fillPortsParameters();
    fillPortsInfo();

    updateSettings();

    initActionsConnections();

    connect(m_serialPort, &QSerialPort::errorOccurred, this, &SerialPortHandler::handleError);
    connect(m_timer, &QTimer::timeout, this, &SerialPortHandler::handleWriteTimeout);
    m_timer->setSingleShot(true);
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPortHandler::readData);
    connect(m_serialPort, &QSerialPort::bytesWritten, this, &SerialPortHandler::handleBytesWritten);

    connect(btn_open, &QPushButton::clicked, this, &SerialPortHandler::open);
    connect(btn_close, &QPushButton::clicked, this, &SerialPortHandler::close);

//    connect(btn_send, &QPushButton::clicked, this, &SerialPortHandler::writeData);
    connect(input_serialPort, &InputTextBox::enterPressed, this, &SerialPortHandler::writeData);

    show();
}

SerialPortHandler::Settings SerialPortHandler::settings() const
{
    return m_currentSettings;
}

SerialPortHandler::~SerialPortHandler()
{

}

void SerialPortHandler::showPortInfo(int idx)
{
    if (idx == -1) return;

    const QString blankString = tr(::blankString);


}

void SerialPortHandler::apply()
{
    updateSettings();
//    ->hide();
}

void SerialPortHandler::checkCustomBaudRatePolicy(int idx)
{

}

void SerialPortHandler::checkCustomDevicePathPolicy(int idx)
{

}

void SerialPortHandler::open()
{
    const SerialPortHandler::Settings p = settings();
    m_serialPort->setPortName(p.name);
    m_serialPort->setBaudRate(p.baudRate);
    m_serialPort->setDataBits(p.dataBits);
    m_serialPort->setParity(p.parity);
    m_serialPort->setStopBits(p.stopBits);
    m_serialPort->setFlowControl(p.flowControl);
    if (m_serialPort->open(QIODevice::ReadWrite)) {
//        m_console->setEnabled(true);
//        m_console->setLocalEchoEnabled(p.localEchoEnabled);
//        m_ui->actionConnect->setEnabled(false);
//        m_ui->actionDisconnect->setEnabled(true);
//        m_ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                              .arg(p.name, p.stringBaudRate, p.stringDataBits,
                                   p.stringParity, p.stringStopBits, p.stringFlowControl));
        emit connected();
    } else {
        QMessageBox::critical(this, tr("Error"), m_serialPort->errorString());

        showStatusMessage(tr("Open error"));
    }
}

void SerialPortHandler::close()
{
    if (m_serialPort->isOpen()) m_serialPort->close();
//    m_console->setEnabled(false);
//    m_ui->actionConnect->setEnabled(true);
//    m_ui->actionDisconnect->setEnabled(false);
//    m_ui->actionConfigure->setEnabled(true);
    showStatusMessage(tr("Disconnected"));

    emit disconnected();
}

void SerialPortHandler::send(QString cmd)
{
    writeData(cmd.toUtf8());
}

void SerialPortHandler::writeData(const QByteArray &data)
{
    const qint64 written = m_serialPort->write(data);
    if (written == data.size()) {
        m_bytesToWrite += written;
        m_timer->start(kWriteTimeout);
    } else {
        const QString error = tr("Failed to write all data to port %1.\n"
                                 "Error: %2").arg(m_serialPort->portName(),
                                                  m_serialPort->errorString());
        showWriteError(error);
    }

    if (chb_localEchoEnabled) {
//        output_serialPort->appendPlainText(data);
    }
}

void SerialPortHandler::readData()
{
    const QByteArray data = m_serialPort->readAll();

    output_serialPort->appendPlainText(data);
}

void SerialPortHandler::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serialPort->errorString());
        close();
    }
}

void SerialPortHandler::handleBytesWritten(qint64 bytes)
{
    m_bytesToWrite -= bytes;
    if (m_bytesToWrite == 0)
        m_timer->stop();
}

void SerialPortHandler::handleWriteTimeout()
{
    const QString error = tr("Write operation timed out for port %1.\n"
                             "Error: %2").arg(m_serialPort->portName(),
                                   m_serialPort->errorString());
    showWriteError(error);
}

void SerialPortHandler::fillPortsParameters()
{
    cb_baudRate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    cb_baudRate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    cb_baudRate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    cb_baudRate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

    cb_dataBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    cb_dataBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    cb_dataBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    cb_dataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    cb_dataBits->setCurrentIndex(3);

    cb_parityBits->addItem(tr("None"), QSerialPort::NoParity);
    cb_parityBits->addItem(tr("Even"), QSerialPort::EvenParity);
    cb_parityBits->addItem(tr("Odd"), QSerialPort::OddParity);
    cb_parityBits->addItem(tr("Mark"), QSerialPort::MarkParity);
    cb_parityBits->addItem(tr("Space"), QSerialPort::SpaceParity);

    cb_stopBits->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    cb_stopBits->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    cb_stopBits->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    cb_flowControl->addItem(tr("None"), QSerialPort::NoFlowControl);
    cb_flowControl->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    cb_flowControl->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SerialPortHandler::fillPortsInfo()
{
    cb_serialPortInfo->clear();
    const QString blankString = tr(::blankString);
    const auto infos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        const QString description = info.description();
        const QString manufacturer = info.manufacturer();
        const QString serialNumber = info.serialNumber();
        const auto vendorId = info.vendorIdentifier();
        const auto productId = info.productIdentifier();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (vendorId ? QString::number(vendorId, 16) : blankString)
             << (productId ? QString::number(productId, 16) : blankString);

        cb_serialPortInfo->addItem(list.constFirst(), list);
    }
}

void SerialPortHandler::updateSettings()
{
    m_currentSettings.name = cb_serialPortInfo->currentText();

    if (cb_baudRate->currentIndex() == 4) {
        m_currentSettings.baudRate = cb_baudRate->currentText().toInt();
    } else {
        const auto baudRateData = cb_baudRate->currentData();
        m_currentSettings.baudRate = baudRateData.value<QSerialPort::BaudRate>();
    }
    m_currentSettings.stringBaudRate = QString::number(m_currentSettings.baudRate);

    const auto dataBitsData = cb_dataBits->currentData();
    m_currentSettings.dataBits = dataBitsData.value<QSerialPort::DataBits>();
    m_currentSettings.stringDataBits = cb_dataBits->currentText();

    const auto parityData = cb_parityBits->currentData();
    m_currentSettings.parity = parityData.value<QSerialPort::Parity>();
    m_currentSettings.stringParity = cb_parityBits->currentText();

    const auto stopBitsData = cb_stopBits->currentData();
    m_currentSettings.stopBits = stopBitsData.value<QSerialPort::StopBits>();
    m_currentSettings.stringStopBits = cb_stopBits->currentText();

    const auto flowControlData = cb_flowControl->currentData();
    m_currentSettings.flowControl = flowControlData.value<QSerialPort::FlowControl>();
    m_currentSettings.stringFlowControl = cb_flowControl->currentText();

    m_currentSettings.localEchoEnabled = chb_localEchoEnabled->isChecked();
}

void SerialPortHandler::initActionsConnections()
{

}

void SerialPortHandler::showStatusMessage(const QString &message)
{
    QMessageBox::information(this, tr("Information"), tr(message.toUtf8()));
}

void SerialPortHandler::showWriteError(const QString &message)
{
    QMessageBox::warning(this, tr("Warning"), message);
}
