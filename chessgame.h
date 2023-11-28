#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <QObject>
#include <QDebug>

#include <algorithm>

class ChessGame : public QObject
{
    Q_OBJECT
public:
    explicit ChessGame(QObject *parent = nullptr);

    void printBoardState();

    void printFEN();
    void setFEN(std::string new_FEN);
    std::string getFEN();

    void printAlgebraicNotation();
    void setAlgebraicNotation(std::vector<std::string> new_chess_algebraic_notation);
    std::vector<std::string> getAlgebraicNotation();

    void setDepth(int new_depth);

    int current_position_idx;
    void increaseCurrentPositionIdx();
    void decreaseCurrentPositionIdx();
    void setCurrentPositionIdx(int idx);
    int getCurrentPositionIdx();

    int latest_position_idx;
    void setLatestPositionIdx(int idx);
    int getLatestPositionIdx();

public slots:
    void getChessboardOutput(const QByteArray &data);
    void getPieceFromField(QString field);
    void resetChessGame();

signals:
    void boardStateChanged(std::vector<std::vector<char>> board_state);
    void sendFENtoStockfish(QByteArray FEN);
    void whoseTurnInfo(bool whoseTurn);

    void movePlayed();
    void initBoardSetupDone();
    void initBoardSetupWait();
    void returnToPositionWait();
    void returnToPositionDone();

    void piece(char piece);

private:
    std::vector<std::vector<char>> board_state{8, std::vector<char>(8, '0')};
    std::vector<std::string> chess_algebraic_notation, short_chess_algebraic_notation;
    const std::string start_pos_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    std::vector<std::string> FEN_game_data;
    std::string FEN = "";

    std::string e_chessboard_position_data = "";
    std::string e_chessboard_move_data = "";

    int depth;

    bool whoseTurn;

    void updateBoardState(std::string FEN);
    void updateBoardState(std::vector<std::string> chess_algebraic_notation);

    std::string getFENfromBoardState();
};

#endif // CHESSGAME_H
