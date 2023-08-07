#include "chessgame.h"

ChessGame::ChessGame(QObject *parent)
    : QObject{parent}
{
    updateBoardState(start_pos_FEN);
    printBoardState();
}

void ChessGame::printBoardState()
{
    std::string row = "";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            row += board_state[i][j];
            row += " ";
        }
        qDebug() << row;
        row = "";
    }
    qDebug() << "\n";
}

void ChessGame::updateBoardState(std::string FEN)
{
    int idx = 0;
    int i = 0;

    while (FEN[idx] != ' ') {
        if (isdigit(FEN[idx])) {
            i += FEN[idx] - '0';
        }
        else if (FEN[idx] != '/') {
            board_state[i/8][i%8] = FEN[idx];
            i++;
        }
        idx++;
    }
}

void ChessGame::updateBoardState(std::vector<std::string> chess_algebraic_notation)
{
    int space_cnt = 0;
    int rank_before = 0, rank_after = 0, file_before = 0, file_after = 0;
    std::string white_move = "", black_move = "";

    for (auto moves : chess_algebraic_notation) {
        for (int i = 0; i < moves.length(); i++) {
            if (moves[i] == ' ') space_cnt++;

            if (space_cnt == 1 && moves[i] != ' ') white_move += moves[i];
            if (space_cnt == 2 && moves[i] != ' ') black_move += moves[i];
        }

        if (std::islower(white_move[0])) {
            file_before = white_move[0] - 'a';
            rank_before = white_move[1] - '1';
            if (std::find(white_move.begin(), white_move.end(), 'x') != white_move.end()) {
                file_after = white_move[3] - 'a';
                rank_after = white_move[4] - '1';
            }
            else {
                file_after = white_move[2] - 'a';
                rank_after = white_move[3] - '1';
            }
        }
        else {
            file_before = white_move[1] - 'a';
            rank_before = white_move[2] - '1';
            if (std::find(white_move.begin(), white_move.end(), 'x') != white_move.end()) {
                file_after = white_move[4] - 'a';
                rank_after = white_move[5] - '1';
            }
            else {
                file_after = white_move[3] - 'a';
                rank_after = white_move[4] - '1';
            }
        }

        board_state[7-rank_after][file_after] = board_state[7-rank_before][file_before];
        board_state[7-rank_before][file_before] = '0';

        if (std::islower(black_move[0])) {
            file_before = black_move[0] - 'a';
            rank_before = black_move[1] - '1';
            if (std::find(black_move.begin(), black_move.end(), 'x') != black_move.end()) {
                file_after = black_move[3] - 'a';
                rank_after = black_move[4] - '1';
            }
            else {
                file_after = black_move[2] - 'a';
                rank_after = black_move[3] - '1';
            }
        }
        else {
            file_before = black_move[1] - 'a';
            rank_before = black_move[2] - '1';
            if (std::find(black_move.begin(), black_move.end(), 'x') != black_move.end()) {
                file_after = black_move[4] - 'a';
                rank_after = black_move[5] - '1';
            }
            else {
                file_after = black_move[3] - 'a';
                rank_after = black_move[4] - '1';
            }
        }

        board_state[7-rank_after][file_after] = board_state[7-rank_before][file_before];
        board_state[7-rank_before][file_before] = '0';

        setFEN(getFENfromBoardState());

        white_move.clear();
        black_move.clear();
        space_cnt = 0;
    }
}

std::string ChessGame::getFENfromBoardState()
{
    std::string current_FEN = "";

    int cnt = 0;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (std::isalpha(board_state[y][x])) {
                if (cnt) {
                    current_FEN += cnt + '0';
                    cnt = 0;
                }
                current_FEN += board_state[y][x];
            }
            else cnt++;
        }

        if (cnt) current_FEN += cnt + '0';
        if (y < 7) current_FEN += "/";

        cnt = 0;
    }

    return current_FEN;
}

void ChessGame::printFEN()
{
    qDebug() << FEN;
}

void ChessGame::setFEN(std::string new_FEN)
{
    FEN = new_FEN;
}

std::string ChessGame::getFEN()
{
    return FEN;
}

void ChessGame::printAlgebraicNotation()
{
    for (auto moves : chess_algebraic_notation) {
        qDebug() << moves;
    }
}

void ChessGame::setAlgebraicNotation(std::vector<std::string> new_chess_algebraic_notation)
{
    chess_algebraic_notation = new_chess_algebraic_notation;
    updateBoardState(chess_algebraic_notation);
}

std::vector<std::string> ChessGame::getAlgebraicNotation()
{
    return chess_algebraic_notation;
}
