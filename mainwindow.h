#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTextEdit>
#include <QSplitter>
#include <QTimer>
#include <QtBluetooth>
#include <QPushButton>
#include <QSerialPort>
#include <QScrollArea>
#include <QScrollBar>

#include "scheduler.h"
#include "stockfish.h"
#include "chessboard.h"
#include "inputtextbox.h"
#include "bluetoothconnectionhandler.h"
#include "chessgame.h"
#include "evaluationbar.h"
#include "objectdetectionhandler.h"
#include "robotcommunicationhandler.h"
#include "serialporthandler.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void startDeviceDiscovery();

signals:
    void bestMoveFound(QString best_move);
    void reset();

private slots:
    void output(QString data);
    void eChessboardOutput(const QByteArray &data);
    void robotCommunicationOutput(QString data);
    void objectDetectionOutput(QString data);
    void setChessPosition(Chessboard* chessboard, ChessGame* chess_game, const QByteArray &data);
    void initiateReset();

    // buttons
//    void btn_forwards_pressed();
//    void btn_backwards_pressed();
//    void btn_start_pressed();
//    void btn_end_pressed();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;

    InputTextBox *itb_stockfish = nullptr;
    InputTextBox *itb_robot_communication = nullptr;

    QPlainTextEdit *stockfish_output = nullptr;
    QPlainTextEdit *eChessboard_output = nullptr;
    QPlainTextEdit *object_detection_output = nullptr;
    QPlainTextEdit *robot_communication_output = nullptr;

    Scheduler *scheduler = nullptr;
    BluetoothConnectionHandler *eChessboard = nullptr;
    Stockfish *stockfish = nullptr;
    Chessboard *chessboard = nullptr;
    ChessGame *chess_game = nullptr;
    ObjectDetectionHandler *object_detection = nullptr;
    RobotCommunicationHandler *robot_communication = nullptr;
    SerialPortHandler *serial_port = nullptr;

    // adding buttons
    QPushButton *btn_forwards = nullptr;
    QPushButton *btn_backwards = nullptr;
    QPushButton *btn_start = nullptr;
    QPushButton *btn_end = nullptr;
};
#endif // MAINWINDOW_H
