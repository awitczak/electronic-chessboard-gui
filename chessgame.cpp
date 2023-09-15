#include "chessgame.h"

ChessGame::ChessGame(QObject *parent)
    : QObject{parent}
{
    FEN_game_data.push_back(start_pos_FEN);
    current_position_idx = 0;

    updateBoardState(start_pos_FEN);
    whoseTurn = true;
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

    // add board position info
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

    // add the turn info
    if (whoseTurn) current_FEN += " w";
    else current_FEN += " b";

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

void ChessGame::setDepth(int new_depth)
{
    depth = new_depth;
}

void ChessGame::increaseCurrentPositionIdx()
{
    qDebug() << current_position_idx;
    current_position_idx++;
}

void ChessGame::decreaseCurrentPositionIdx()
{
    current_position_idx--;
}

void ChessGame::setCurrentPositionIdx(int idx)
{
    current_position_idx = idx;
}

int ChessGame::getCurrentPositionIdx()
{
    return current_position_idx;
}

void ChessGame::setLatestPositionIdx(int idx)
{
    latest_position_idx = idx;
}

int ChessGame::getLatestPositionIdx()
{
    return latest_position_idx;
}

void ChessGame::resetChessGame()
{
    FEN_game_data.clear();
    FEN_game_data.push_back(start_pos_FEN);
    current_position_idx = 0;

    updateBoardState(start_pos_FEN);
    setFEN(start_pos_FEN);
    whoseTurn = true;
}

void ChessGame::getChessboardOutput(const QByteArray &data)
{
    QString chessboardData = QString::fromUtf8(data);

    QStringList dataList = chessboardData.split('#');

    if (dataList[0] == "1") {

        whoseTurn = !whoseTurn;

        e_chessboard_position_data = dataList[1].toStdString();
        short_chess_algebraic_notation.push_back(dataList[2].toStdString());
        e_chessboard_move_data = dataList[3].toStdString();

        // position data to board_state
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // sigh - temporary, as the firmware of the chessboard remains unchanged
                int idx = i * 8 + j;
                if (e_chessboard_position_data[idx] == 'a') board_state[i][j] = 'p';
                else if (e_chessboard_position_data[idx] == 'b') board_state[i][j] = 'n';
                else if (e_chessboard_position_data[idx] == 'c') board_state[i][j] = 'b';
                else if (e_chessboard_position_data[idx] == 'd') board_state[i][j] = 'r';
                else if (e_chessboard_position_data[idx] == 'e') board_state[i][j] = 'q';
                else if (e_chessboard_position_data[idx] == 'f') board_state[i][j] = 'k';
                else if (e_chessboard_position_data[idx] == '1') board_state[i][j] = 'P';
                else if (e_chessboard_position_data[idx] == '2') board_state[i][j] = 'N';
                else if (e_chessboard_position_data[idx] == '3') board_state[i][j] = 'B';
                else if (e_chessboard_position_data[idx] == '4') board_state[i][j] = 'R';
                else if (e_chessboard_position_data[idx] == '5') board_state[i][j] = 'Q';
                else if (e_chessboard_position_data[idx] == '6') board_state[i][j] = 'K';
                else board_state[i][j] = '0';
            }
        }

        // FEN from board_state
        setFEN(getFENfromBoardState());

        emit boardStateChanged(board_state);
        emit sendFENtoStockfish(QByteArray::fromStdString(FEN));
        emit whoseTurnInfo(whoseTurn);
    }
}
