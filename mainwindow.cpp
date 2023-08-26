#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    showMaximized();


    Stockfish *stockfish = new Stockfish();



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

    leftWidget->setStyleSheet("background-color: rgb(200, 200, 200);");
    rightWidget->setStyleSheet("background-color: rgb(200, 200, 200);");

    QWidget *leftWidgetFirstRow = new QWidget(this);

    QHBoxLayout *leftWidgetFirstRowLayout = new QHBoxLayout(leftWidgetFirstRow);
    leftWidgetLayout->addWidget(leftWidgetFirstRow);

    Chessboard *chessboard = new Chessboard(this);
    EvaluationBar *evaluationBar = new EvaluationBar(this);

    leftWidgetFirstRowLayout->addWidget(evaluationBar);
    leftWidgetFirstRowLayout->addWidget(chessboard);

    leftWidgetFirstRow->setLayout(leftWidgetFirstRowLayout);
    leftWidgetFirstRowLayout->setSpacing(0);

    leftWidgetFirstRow->setStyleSheet("background-color: rgb(100, 100, 100);");

    // adding buttons for forwards/backwards game viewing
    QPushButton *btn_forwards = new QPushButton(this);
    QPushButton *btn_backwards = new QPushButton(this);
    leftWidgetLayout->addWidget(btn_forwards);
    leftWidgetLayout->addWidget(btn_backwards);

//    // create different tabs for the right widget
//    QTabWidget* tabWidget = new QTabWidget(rightWidget);

//    QWidget* tab1 = new QWidget(tabWidget);
//    QWidget* tab2 = new QWidget(tabWidget);
//    QWidget* tab3 = new QWidget(tabWidget);

//    tabWidget->addTab(tab1, "tab 1");
//    tabWidget->addTab(tab2, "tab 2");
//    tabWidget->addTab(tab3, "tab 3");

    // tab 1 - engine connection
    QVBoxLayout *rightWidgetLayout = new QVBoxLayout(rightWidget);

    engine_output = new QTextEdit(this);
    engine_output->setMinimumSize(400, 100);
    engine_output->setStyleSheet("background-color: rgb(255, 255, 255);");

    // send commands to engine
    InputTextBox *inputTextBox_send = new InputTextBox(this);
    inputTextBox_send->setMinimumSize(400, 30);
    inputTextBox_send->setMaximumHeight(30);
    inputTextBox_send->setStyleSheet("background-color: rgb(255, 255, 255);");

    // send commands to chessboard
    InputTextBox *inputTextBox_send2 = new InputTextBox(this);
    inputTextBox_send2->setMinimumSize(400, 30);
    inputTextBox_send2->setMaximumHeight(30);
    inputTextBox_send2->setStyleSheet("background-color: rgb(255, 255, 255);");

    // send an algebraic chess notation for the program to set
    InputTextBox *inputTextBox_send3 = new InputTextBox(this);
    inputTextBox_send3->setMinimumSize(400, 30);
    inputTextBox_send3->setMaximumHeight(30);
    inputTextBox_send3->setStyleSheet("background-color: rgb(255, 255, 255);");

    // a text box with data received from the e-chessboard
    eChessboard_output = new QTextEdit(this);
    eChessboard_output->setMinimumSize(400, 100);
    eChessboard_output->setStyleSheet("background-color: rgb(255, 255, 255);");

    // button to connect/disconnect e-chessboard
    QPushButton *btn_connect_bluetooth = new QPushButton(rightWidget);
    btn_connect_bluetooth->setText("Connect");

    rightWidgetLayout->addWidget(engine_output);
    rightWidgetLayout->addWidget(inputTextBox_send);
    rightWidgetLayout->addWidget(inputTextBox_send2);
    rightWidgetLayout->addWidget(inputTextBox_send3);
    rightWidgetLayout->addWidget(eChessboard_output);
    rightWidgetLayout->addWidget(btn_connect_bluetooth);



    ChessGame *chess_game = new ChessGame(this);










    // signals
    connect(stockfish, &Stockfish::output, this, &MainWindow::output);
    connect(inputTextBox_send, &InputTextBox::enterPressed, stockfish, [stockfish, inputTextBox_send](){stockfish->send(inputTextBox_send->toPlainText().toUtf8());});
    connect(inputTextBox_send2, &InputTextBox::enterPressed, this, [this, chessboard, inputTextBox_send2]() {handle_FEN_textEdit_Pressed(chessboard, inputTextBox_send2->toPlainText());});

    connect(chessboard, &Chessboard::resizeEvalBar, evaluationBar, &EvaluationBar::resizeEvalBar);

    // bluetooth handling
    BluetoothConnectionHandler *connection = new BluetoothConnectionHandler();
//    connection->startDeviceDiscovery();

    connect(connection, &BluetoothConnectionHandler::dataReceived, this, &MainWindow::eChessboardOutput);

    connect(btn_connect_bluetooth, &QPushButton::pressed, connection, &BluetoothConnectionHandler::startDeviceDiscovery);





    connect(connection, &BluetoothConnectionHandler::dataReceived, chess_game, &ChessGame::getChessboardOutput);

    connect(inputTextBox_send3, &InputTextBox::enterPressed, this, [this, chessboard, chess_game, inputTextBox_send3]() {setChessPosition(chessboard, chess_game, inputTextBox_send3->toPlainText().toUtf8());});

    connect(this, &MainWindow::updateEvalBar, evaluationBar, &EvaluationBar::updateEvalBar);

    connect(chess_game, &ChessGame::boardStateChanged, chessboard, &Chessboard::updateChessboard);

    connect(this, &MainWindow::bestMoveFound, chessboard, &Chessboard::showBestMove);

    connect(chess_game, &ChessGame::sendFENtoStockfish, stockfish, &Stockfish::updateFEN);

    connect(chess_game, &ChessGame::whoseTurnInfo, this, &MainWindow::getWhoseTurnInfo);

    // reset chessboard gui, chess game info and stockfish
    connect(this, &MainWindow::reset, chessboard, &Chessboard::resetChessboard);
    connect(this, &MainWindow::reset, chess_game, &ChessGame::resetChessGame);
    connect(this, &MainWindow::reset, stockfish, &Stockfish::resetStockfish);
    connect(this, &MainWindow::reset, evaluationBar, &EvaluationBar::resetEvalBar);

    connect(btn_connect_bluetooth, &QPushButton::pressed, this, &MainWindow::initiateReset);

    // engine handling
    stockfish->start();


    statusBar()->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::output(QString data)
{
    engine_output->append(data);

    if (data.contains("bestmove")) {

        int bestmove_last_idx = data.indexOf("bestmove ") + strlen("bestmove ");
        int ponder_idx = data.indexOf("ponder");

        QString best_move = data.mid(bestmove_last_idx, ponder_idx - 1 - bestmove_last_idx);

        emit bestMoveFound(best_move);
    }

    emit updateEvalBar(data, whoseTurn);
}

void MainWindow::handle_FEN_textEdit_Pressed(Chessboard* chessboard, QString FEN)
{
//    chessboard->Chessboard::setBoardStateFromFEN(FEN.toUtf8());
//    chessboard->updateChessboard();

//    // send the FEN to the engine too
//    m_stockfish.send("position fen " + FEN.toUtf8());
//    m_stockfish.send("go depth 25");
}

void MainWindow::eChessboardOutput(const QByteArray &data)
{
    eChessboard_output->append(data);
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
    whoseTurn = true;

    emit reset();
}

void MainWindow::getWhoseTurnInfo(bool turnInfo)
{
    whoseTurn = turnInfo;
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


