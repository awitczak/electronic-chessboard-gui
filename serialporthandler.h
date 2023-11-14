#ifndef SERIALPORTHANDLER_H
#define SERIALPORTHANDLER_H

#include <QObject>
#include <QSerialPort>
#include <QDialog>
#include <QPushButton>
#include <QComboBox>
#include <QLayout>
#include <QSerialPortInfo>
#include <QCheckBox>
#include <QMessageBox>
#include <QTimer>
#include <QPlainTextEdit>
#include "inputtextbox.h"

#include <chrono>

class SerialPortHandler : public QWidget
{
    Q_OBJECT
public:
    explicit SerialPortHandler(QWidget *parent = nullptr);

    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };

    Settings settings() const;

    ~SerialPortHandler();

signals:

private slots:
    // settings dialog
    void showPortInfo(int idx);
    void apply();
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);

    // serial port handler
    void open();
    void close();
    void send();
    void writeData(const QByteArray &data);
    void readData();

    void handleError(QSerialPort::SerialPortError error);
    void handleBytesWritten(qint64 bytes);
    void handleWriteTimeout();

private:
    // settings dialog handler
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

    // serial port
    void initActionsConnections();
    void showStatusMessage(const QString &message);
    void showWriteError(const QString &message);

private:
    // settings
    Settings m_currentSettings;

    QWidget *serialPort = nullptr;

    QPushButton *btn_apply = nullptr;
    QComboBox *cb_serialPortInfo = nullptr;
    QComboBox *cb_baudRate = nullptr;
    QComboBox *cb_dataBits = nullptr;
    QComboBox *cb_parityBits = nullptr;
    QComboBox *cb_stopBits = nullptr;
    QComboBox *cb_flowControl = nullptr;

    QCheckBox *chb_localEchoEnabled = nullptr;

    // serial port handler
    qint64 m_bytesToWrite = 0;
    QTimer *m_timer = nullptr;
    QSerialPort *m_serialPort = nullptr;

    QPlainTextEdit *output_serialPort = nullptr;
    InputTextBox *input_serialPort = nullptr;
    QPushButton *btn_send = nullptr;
    QPushButton *btn_open = nullptr;
    QPushButton *btn_close = nullptr;
};

#endif // SERIALPORTHANDLER_H
