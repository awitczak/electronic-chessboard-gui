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

    void updateField(const int X, const int Y, const char piece);
    void updateFieldBackground(const int X, const int Y, const QString style);
    void setBoardStateFromFEN(const char* FEN_string);
    void print_board_state();
    QString getPieceFromLetter(const char pieceLetter);
    void showBestMove(QString best_move);

public slots:
    void updateChessboard(std::vector<std::vector<char>> board_state);
    void resetChessboard();

private:
    const char* start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::vector<std::vector<char>> board_state{8, std::vector<char>(8, '0')};

    int past_best_move[4];

    QGridLayout *chessboard;

    void createChessboard();
    void clearBestMoveHistory();
    void repaintChessboard();

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void resizeEvalBar(int height);
};

#endif // CHESSBOARD_H
