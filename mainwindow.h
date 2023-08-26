#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QTextEdit>
#include <QSplitter>
#include <QTimer>
#include <QtBluetooth>
#include <QPushButton>

#include "stockfish.h"
#include "chessboard.h"
#include "inputtextbox.h"
#include "bluetoothconnectionhandler.h"
#include "chessgame.h"
#include "evaluationbar.h"


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
    void updateEvalBar(QString eval, bool whoseTurn);
    void bestMoveFound(QString best_move);
    void reset();

private slots:
    void output(QString data);
    void handle_FEN_textEdit_Pressed(Chessboard* chessboard, QString FEN);
    void eChessboardOutput(const QByteArray &data);
    void setChessPosition(Chessboard* chessboard, ChessGame* chess_game, const QByteArray &data);
    void initiateReset();
    void getWhoseTurnInfo(bool turnInfo);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;

    QTextEdit *engine_output;
    QTextEdit *eChessboard_output;

    Stockfish *stockfish;
    Chessboard *chessboard;
    ChessGame *chess_game;

    bool whoseTurn = true;
};
#endif // MAINWINDOW_H
