#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QFrame>
#include <QDebug>
#include <QResizeEvent>

#include "chessboardsquare.h"

class Chessboard : public QWidget
{
    Q_OBJECT

public:
    Chessboard(QWidget *parent = nullptr);
    void updateChessboard();
    void updateField(const int X, const int Y, const char piece);
    void setBoardStateFromFEN(const char* FEN_string);
    void setPosition(const char* FEN_string);
    void print_board_state();
    QString getPieceFromLetter(const char pieceLetter);


private:
    const char* start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    static const int N_squares = 64;
    char board_state[N_squares];

    QGridLayout *chessboard;

    void createChessboard();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void resizeEvalBar(int height);
};

#endif // CHESSBOARD_H
