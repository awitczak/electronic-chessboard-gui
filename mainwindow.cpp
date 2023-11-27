#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scheduler = new Scheduler();

    stockfish = new Stockfish();
    chess_game = new ChessGame();
    eChessboard = new BluetoothConnectionHandler();
    robot_communication = new RobotCommunicationHandler();
    object_detection = new ObjectDetectionHandler();
    serial_port = new SerialPortHandler();

    // connecting stockfish
    connect(stockfish, &Stockfish::connected, scheduler, &Scheduler::stockfishConnected);
    connect(stockfish, &Stockfish::disconnected, scheduler, &Scheduler::stockfishDisconnected);

    // connecting eChessboard
    connect(eChessboard, &BluetoothConnectionHandler::connected, scheduler, &Scheduler::eChessboardConnected);
    connect(scheduler, &Scheduler::eChessboardDisconnected, eChessboard, &BluetoothConnectionHandler::disconnected);

    // connecting ChessGame
    connect(chess_game, &ChessGame::initBoardSetupDone, scheduler, &Scheduler::eChessboardInitBoardSetupDone);
    connect(chess_game, &ChessGame::initBoardSetupWait, scheduler, &Scheduler::eChessboardInitBoardSetupWait);
    connect(chess_game, &ChessGame::returnToPositionDone, scheduler, &Scheduler::eChessboardReturnToPositionDone);
    connect(chess_game, &ChessGame::returnToPositionWait, scheduler, &Scheduler::eChessboardReturnToPositionWait);
    connect(chess_game, &ChessGame::movePlayed, scheduler, &Scheduler::eChessboardMovePlayed);

    // connecting object_detection
    connect(object_detection, &ObjectDetectionHandler::connected, scheduler, &Scheduler::objectDetectionConnected);
    connect(object_detection, &ObjectDetectionHandler::disconnected, scheduler, &Scheduler::objectDetectionDisconnected);
    connect(object_detection, &ObjectDetectionHandler::personDetected, scheduler, &Scheduler::personDetected);
    connect(object_detection, &ObjectDetectionHandler::personNotDetected, scheduler, &Scheduler::personNotDetected);

    // connecting gripper
    connect(serial_port, &SerialPortHandler::connected, scheduler, &Scheduler::gripperConnected);
    connect(serial_port, &SerialPortHandler::disconnected, scheduler, &Scheduler::gripperDisconnected);
    connect(serial_port, &SerialPortHandler::ready, scheduler, &Scheduler::gripperReady);
    connect(serial_port, &SerialPortHandler::busy, scheduler, &Scheduler::gripperBusy);
    connect(scheduler, &Scheduler::sendGripperCommand, serial_port, &SerialPortHandler::send);

    // connecting robotCom
    connect(robot_communication, &RobotCommunicationHandler::connected, scheduler, &Scheduler::robotComConnected);
    connect(robot_communication, &RobotCommunicationHandler::disconnected, scheduler, &Scheduler::robotComDisconnected);
    connect(robot_communication, &RobotCommunicationHandler::moving, scheduler, &Scheduler::robotComMoving);
    connect(robot_communication, &RobotCommunicationHandler::notMoving, scheduler, &Scheduler::robotComNotMoving);
    connect(scheduler, &Scheduler::sendRobotCommand, robot_communication, &RobotCommunicationHandler::send);



    // -------------------------- UI -----------------------------

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(splitter);

    QWidget *leftWidget = new QWidget(this);
    QWidget *rightWidget = new QWidget(this);

    QVBoxLayout *leftWidgetLayout = new QVBoxLayout(leftWidget);

    splitter->addWidget(leftWidget);
    splitter->addWidget(rightWidget);

    int largeHeight = QGuiApplication::primaryScreen()->virtualSize().height();
    splitter->setSizes(QList<int>({largeHeight , largeHeight}));

    leftWidget->setStyleSheet("background-color: rgb(180, 170, 185);");
    rightWidget->setStyleSheet("background-color: rgb(180, 170, 185);");

    QWidget *leftWidgetFirstRow = new QWidget(leftWidget);

    QHBoxLayout *leftWidgetFirstRowLayout = new QHBoxLayout(leftWidgetFirstRow);
    leftWidgetLayout->addWidget(leftWidgetFirstRow);

    Chessboard *chessboard = new Chessboard(leftWidgetFirstRow);
    EvaluationBar *evaluationBar = new EvaluationBar(leftWidgetFirstRow);

    leftWidgetFirstRowLayout->addWidget(evaluationBar);
    leftWidgetFirstRowLayout->addWidget(chessboard);

    leftWidgetFirstRow->setLayout(leftWidgetFirstRowLayout);

    QWidget *leftWidgetSecondRow = new QWidget(leftWidget);

    QHBoxLayout *leftWidgetSecondRowLayout = new QHBoxLayout(leftWidgetSecondRow);
    leftWidgetLayout->addWidget(leftWidgetSecondRow);

    // adding buttons
    QPushButton *btn_forwards = new QPushButton(leftWidgetSecondRow);
    QPushButton *btn_backwards = new QPushButton(leftWidgetSecondRow);
    QPushButton *btn_start = new QPushButton(leftWidgetSecondRow);
    QPushButton *btn_end = new QPushButton(leftWidgetSecondRow);

    btn_forwards->setText(">");
    btn_backwards->setText("<");
    btn_start->setText("<<");
    btn_end->setText(">>");

    leftWidgetSecondRowLayout->addWidget(btn_start);
    leftWidgetSecondRowLayout->addWidget(btn_backwards);
    leftWidgetSecondRowLayout->addWidget(btn_forwards);
    leftWidgetSecondRowLayout->addWidget(btn_end);

    // right widget
    QVBoxLayout *rightWidgetLayout = new QVBoxLayout(rightWidget);

    // create different tabs for the right widget
    QTabWidget* tabWidget = new QTabWidget();

    QScrollArea* tab1 = new QScrollArea(tabWidget);
    QScrollArea* tab2 = new QScrollArea(tabWidget);
    QScrollArea* tab3 = new QScrollArea(tabWidget);
    QScrollArea* tab4 = new QScrollArea(tabWidget);
    QScrollArea* tab5 = new QScrollArea(tabWidget);

    tabWidget->addTab(tab1, "Debug 1");
    tabWidget->addTab(tab2, "Debug 2");
    tabWidget->addTab(tab3, "Debug 3");
    tabWidget->addTab(tab4, "Debug 4");
    tabWidget->addTab(tab5, "Debug 5");

    QWidget* tab1_widget = new QWidget(tab1);
    QWidget* tab2_widget = new QWidget(tab2);
    QWidget* tab3_widget = new QWidget(tab3);
    QWidget* tab4_widget = new QWidget(tab4);
    QWidget* tab5_widget = new QWidget(tab5);

    QVBoxLayout *tab1WidgetLayout = new QVBoxLayout(tab1_widget);
    QVBoxLayout *tab2WidgetLayout = new QVBoxLayout(tab2_widget);
    QVBoxLayout *tab3WidgetLayout = new QVBoxLayout(tab3_widget);
    QVBoxLayout *tab4WidgetLayout = new QVBoxLayout(tab4_widget);
    QVBoxLayout *tab5WidgetLayout = new QVBoxLayout(tab5_widget);

    tab1->setWidget(tab1_widget);
    tab1->setWidgetResizable(true);
    tab1->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tab1->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    tab2->setWidget(tab2_widget);
    tab2->setWidgetResizable(true);
    tab2->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tab2->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    tab3->setWidget(tab3_widget);
    tab3->setWidgetResizable(true);
    tab3->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tab3->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    tab4->setWidget(tab4_widget);
    tab4->setWidgetResizable(true);
    tab4->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tab4->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    tab5->setWidget(tab5_widget);
    tab5->setWidgetResizable(true);
    tab5->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    tab5->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    rightWidgetLayout->addWidget(tabWidget);

    stockfish_output = new QPlainTextEdit();
    stockfish_output->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical {"
        "    border: 2px solid #999999;"
        "    background: #f0f0f0;"
        "    width: 16px;"
        "    margin: 22px 0 22px 0;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #666666;"
        "    min-height: 20px;"
        "}"
        "QScrollBar::add-line:vertical {"
        "    border: 2px solid #999999;"
        "    background: #f0f0f0;"
        "    height: 20px;"
        "    subcontrol-position: bottom;"
        "    subcontrol-origin: margin;"
        "}"
        "QScrollBar::sub-line:vertical {"
        "    border: 2px solid #999999;"
        "    background: #f0f0f0;"
        "    height: 20px;"
        "    subcontrol-position: top;"
        "    subcontrol-origin: margin;"
        "}"
       );
    stockfish_output->setMinimumSize(400, 100);
    stockfish_output->setStyleSheet("background-color: rgb(255, 255, 255);");

    // send commands to engine
    itb_stockfish = new InputTextBox(this);
    itb_stockfish->setMinimumSize(400, 30);
    itb_stockfish->setMaximumHeight(30);
    itb_stockfish->setStyleSheet("background-color: rgb(255, 255, 255);");

//    // send an algebraic chess notation for the program to set
//    InputTextBox *inputTextBox_send3 = new InputTextBox(this);
//    inputTextBox_send3->setMinimumSize(400, 30);
//    inputTextBox_send3->setMaximumHeight(30);
//    inputTextBox_send3->setStyleSheet("background-color: rgb(255, 255, 255);");

    // a text box with data received from the e-chessboard
    eChessboard_output = new QPlainTextEdit(this);
    eChessboard_output->setMinimumSize(400, 100);
    eChessboard_output->setStyleSheet("background-color: rgb(255, 255, 255);");

    // button to connect/disconnect e-chessboard
    QPushButton *btn_connect_bluetooth = new QPushButton(rightWidget);
    btn_connect_bluetooth->setText("Connect");

    // labels
    QLabel *lbl_stockfish_output = new QLabel(rightWidget);
    lbl_stockfish_output->setText("Engine output:");

    QLabel *lbl_eChessboard_output = new QLabel(rightWidget);
    lbl_eChessboard_output->setText("e-Chessboard output:");

    tab1WidgetLayout->addWidget(lbl_stockfish_output);
    tab1WidgetLayout->addWidget(stockfish_output);
    tab1WidgetLayout->addWidget(itb_stockfish);

//    rightWidgetLayout->addWidget(inputTextBox_send3);

    tab2WidgetLayout->addWidget(lbl_eChessboard_output);
    tab2WidgetLayout->addWidget(eChessboard_output);
    tab2WidgetLayout->addWidget(btn_connect_bluetooth);

    // connecting the object detection script
    QPushButton *btn_start_object_detection = new QPushButton(this);
    QPushButton *btn_stop_object_detection = new QPushButton(this);

    btn_start_object_detection->setText("start-object-det");
    btn_stop_object_detection->setText("stop-object-det");

    QLabel *lbl_object_detection_output = new QLabel(rightWidget);
    lbl_object_detection_output->setText("Object detection output:");

    object_detection_output = new QPlainTextEdit(this);
    object_detection_output->setMinimumSize(400, 100);
    object_detection_output->setStyleSheet("background-color: rgb(255, 255, 255);");

    tab3WidgetLayout->addWidget(btn_start_object_detection);
    tab3WidgetLayout->addWidget(btn_stop_object_detection);
    tab3WidgetLayout->addWidget(lbl_object_detection_output);
    tab3WidgetLayout->addWidget(object_detection_output);

    object_detection->setPath("/home/adam/Desktop/object_detection/human_detection.py");

    connect(btn_start_object_detection, &QPushButton::pressed, object_detection, &ObjectDetectionHandler::start);
    connect(btn_stop_object_detection, &QPushButton::pressed, object_detection, &ObjectDetectionHandler::stop);
    connect(object_detection, &ObjectDetectionHandler::output, this, &MainWindow::objectDetectionOutput);

    // connecting the robot communication script
    QPushButton *btn_start_robot_communication = new QPushButton(this);
    QPushButton *btn_stop_robot_communication = new QPushButton(this);

    btn_start_robot_communication->setText("start-robot-com");
    btn_stop_robot_communication->setText("stop-robot-com");

    QLabel *lbl_robot_communication_output = new QLabel(rightWidget);
    lbl_robot_communication_output->setText("Robot communication:");

    robot_communication_output = new QPlainTextEdit();
    robot_communication_output->setMinimumSize(400, 100);
    robot_communication_output->setStyleSheet("background-color: rgb(255, 255, 255);");

    itb_robot_communication = new InputTextBox();
    itb_robot_communication->setMinimumSize(400, 30);
    itb_robot_communication->setMaximumHeight(30);
    itb_robot_communication->setStyleSheet("background-color: rgb(255, 255, 255);");

    // adding buttons
    QWidget *controlPanel = new QWidget();
    QHBoxLayout *controlPanelLayout = new QHBoxLayout(controlPanel);

    QPushButton *btn_positiveX = new QPushButton();
    QPushButton *btn_negativeX = new QPushButton();
    QPushButton *btn_positiveY = new QPushButton();
    QPushButton *btn_negativeY = new QPushButton();
    QPushButton *btn_positiveZ = new QPushButton();
    QPushButton *btn_negativeZ = new QPushButton();
    cb_distances = new QComboBox();
    QPushButton *btn_setA1Corner = new QPushButton();

    btn_positiveX->setText("X+");
    btn_negativeX->setText("X-");
    btn_positiveY->setText("Y+");
    btn_negativeY->setText("Y-");
    btn_positiveZ->setText("Z+");
    btn_negativeZ->setText("Z-");
    cb_distances->addItems({QStringLiteral("0.1"), QStringLiteral("0.5"), QStringLiteral("1"), QStringLiteral("5"), QStringLiteral("10")});
    btn_setA1Corner->setText("set A1 corner");

    controlPanelLayout->addWidget(btn_negativeX);
    controlPanelLayout->addWidget(btn_positiveX);
    controlPanelLayout->addWidget(btn_negativeY);
    controlPanelLayout->addWidget(btn_positiveY);
    controlPanelLayout->addWidget(btn_negativeZ);
    controlPanelLayout->addWidget(btn_positiveZ);
    controlPanelLayout->addWidget(cb_distances);
    controlPanelLayout->addWidget(btn_setA1Corner);

    connect(btn_positiveX, &QPushButton::pressed, this, &MainWindow::btn_positiveX_pressed);
    connect(btn_negativeX, &QPushButton::pressed, this, &MainWindow::btn_negativeX_pressed);
    connect(btn_positiveY, &QPushButton::pressed, this, &MainWindow::btn_positiveY_pressed);
    connect(btn_negativeY, &QPushButton::pressed, this, &MainWindow::btn_negativeY_pressed);
    connect(btn_positiveZ, &QPushButton::pressed, this, &MainWindow::btn_positiveZ_pressed);
    connect(btn_negativeZ, &QPushButton::pressed, this, &MainWindow::btn_negativeZ_pressed);
    connect(btn_setA1Corner, &QPushButton::pressed, this, &MainWindow::btn_setA1Corner_pressed);
    connect(this, &MainWindow::setCornerPos_A1, robot_communication, &RobotCommunicationHandler::setChessboardA1CornerPos);

    tab4WidgetLayout->addWidget(lbl_robot_communication_output);
    tab4WidgetLayout->addWidget(btn_start_robot_communication);
    tab4WidgetLayout->addWidget(btn_stop_robot_communication);
    tab4WidgetLayout->addWidget(robot_communication_output);
    tab4WidgetLayout->addWidget(itb_robot_communication);
    tab4WidgetLayout->addWidget(controlPanel);

    robot_communication->setPath("/home/adam/Desktop/robot-control/build/robot-control");

    connect(btn_start_robot_communication, &QPushButton::pressed, robot_communication, &RobotCommunicationHandler::start);
    connect(btn_stop_robot_communication, &QPushButton::pressed, robot_communication, &RobotCommunicationHandler::stop);
    connect(robot_communication, &RobotCommunicationHandler::output, this, &MainWindow::robotCommunicationOutput);
    connect(itb_robot_communication, &InputTextBox::enterPressed, robot_communication, &RobotCommunicationHandler::send);

    tab5WidgetLayout->addWidget(serial_port);







    // signals
    connect(stockfish, &Stockfish::output, this, &MainWindow::stockfishOutput);
    connect(itb_stockfish, &InputTextBox::enterPressed, stockfish, &Stockfish::send);

    connect(chessboard, &Chessboard::resizeEvalBar, evaluationBar, &EvaluationBar::resizeEvalBar);

    // bluetooth handling

//    connection->startDeviceDiscovery();

    connect(eChessboard, &BluetoothConnectionHandler::dataReceived, this, &MainWindow::eChessboardOutput);

    connect(btn_connect_bluetooth, &QPushButton::pressed, eChessboard, &BluetoothConnectionHandler::startDeviceDiscovery);





    connect(eChessboard, &BluetoothConnectionHandler::dataReceived, chess_game, &ChessGame::getChessboardOutput);

    connect(stockfish, &Stockfish::mateEvaluated, evaluationBar, &EvaluationBar::mateEvaluated);
    connect(stockfish, &Stockfish::currentEvaluation, evaluationBar, &EvaluationBar::updateEvalBar);

    connect(chess_game, &ChessGame::boardStateChanged, chessboard, &Chessboard::updateChessboard);

    connect(this, &MainWindow::bestMoveFound, chessboard, &Chessboard::showBestMove);

    connect(chess_game, &ChessGame::sendFENtoStockfish, stockfish, &Stockfish::updateFEN);

    connect(chess_game, &ChessGame::whoseTurnInfo, evaluationBar, &EvaluationBar::flipTurns);

    // reset chessboard gui, chess game info and stockfish
    connect(this, &MainWindow::reset, chessboard, &Chessboard::resetChessboard);
    connect(this, &MainWindow::reset, chess_game, &ChessGame::resetChessGame);
    connect(this, &MainWindow::reset, stockfish, &Stockfish::resetStockfish);
    connect(this, &MainWindow::reset, evaluationBar, &EvaluationBar::resetEvalBar);
    connect(this, &MainWindow::reset, scheduler, &Scheduler::reset);

    connect(btn_connect_bluetooth, &QPushButton::pressed, this, &MainWindow::initiateReset);


    // button press handling
//    connect(btn_start, &QPushButton::pressed, this, &MainWindow::btn_start_pressed);
//    connect(btn_end, &QPushButton::pressed, this, &MainWindow::btn_end_pressed);
//    connect(btn_forwards, &QPushButton::pressed, this, &MainWindow::btn_forwards_pressed);
//    connect(btn_backwards, &QPushButton::pressed, this, &MainWindow::btn_backwards_pressed);


    // engine handling
    stockfish->start();

    statusBar()->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::stockfishOutput(QString data)
{
    stockfish_output->appendPlainText(data);
}

void MainWindow::eChessboardOutput(const QByteArray &data)
{
    eChessboard_output->appendPlainText(data);
}

void MainWindow::robotCommunicationOutput(QString data)
{
    robot_communication_output->appendPlainText(data);
}

void MainWindow::objectDetectionOutput(QString data)
{
    if (data.endsWith('\n')) data.chop(1);
    object_detection_output->appendPlainText(data);
}


void MainWindow::setChessPosition(Chessboard *chessboard, ChessGame *chess_game, const QByteArray &data)
{
    std::string dataString = data.toStdString();
    std::vector<std::string> new_chess_algebraic_notation;

    qDebug() << dataString;

    std::string line = "";
    for (int i = 0; i < dataString.length(); i++) {
        if (line.length() > 0 && std::isdigit(dataString[i]) && dataString[i+1] == '.') {
            new_chess_algebraic_notation.push_back(line);
            line = "";
        }
        line += dataString[i];
    }
    new_chess_algebraic_notation.push_back(line);

    chess_game->setAlgebraicNotation(new_chess_algebraic_notation);
    chess_game->printBoardState();
    chess_game->printFEN();
}

void MainWindow::initiateReset()
{
    emit reset();
}

void MainWindow::btn_positiveX_pressed()
{
    float val = cb_distances->currentText().toFloat() / 100;
    QByteArray bq_val;
    bq_val.setNum(val);

    QByteArray cmd = "move_relative " + bq_val + " 0 0 0 0 0";
    robot_communication->send(cmd);
}

void MainWindow::btn_negativeX_pressed()
{
    float val = cb_distances->currentText().toFloat() / 100;
    QByteArray bq_val;
    bq_val.setNum(val);

    QByteArray cmd = "move_relative -" + bq_val + " 0 0 0 0 0";
    robot_communication->send(cmd);
}

void MainWindow::btn_positiveY_pressed()
{
    float val = cb_distances->currentText().toFloat() / 100;
    QByteArray bq_val;
    bq_val.setNum(val);

    QByteArray cmd = "move_relative 0 " + bq_val + " 0 0 0 0";
    robot_communication->send(cmd);
}

void MainWindow::btn_negativeY_pressed()
{
    float val = cb_distances->currentText().toFloat() / 100;
    QByteArray bq_val;
    bq_val.setNum(val);

    QByteArray cmd = "move_relative 0 -" + bq_val + " 0 0 0 0";
    robot_communication->send(cmd);
}

void MainWindow::btn_positiveZ_pressed()
{
    float val = cb_distances->currentText().toFloat() / 100;
    QByteArray bq_val;
    bq_val.setNum(val);

    QByteArray cmd = "move_relative 0 0 " + bq_val + " 0 0 0";
    robot_communication->send(cmd);
}

void MainWindow::btn_negativeZ_pressed()
{
    float val = cb_distances->currentText().toFloat() / 100;
    QByteArray bq_val;
    bq_val.setNum(val);

    QByteArray cmd = "move_relative 0 0 -" + bq_val + " 0 0 0";
    robot_communication->send(cmd);
}

void MainWindow::btn_setA1Corner_pressed()
{
    emit setCornerPos_A1();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        close();
    }
    else {
        QMainWindow::keyPressEvent(event);
    }
}


