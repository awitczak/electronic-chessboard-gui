#include "chessboard.h"

Chessboard::Chessboard(QWidget *parent) : QWidget(parent)
{
    clearBestMoveHistory();
    createChessboard();

    setMinimumSize(800, 800);
}

void Chessboard::createChessboard()
{
    setBoardStateFromFEN(start_pos);

    chessboard = new QGridLayout(this);

    int grid_size = 8;
    for (int y = 0; y < grid_size; y++) {
        for (int x = 0; x < grid_size; x++) {

            ChessboardSquare *square = new ChessboardSquare();

            square->setIcon(getPieceFromLetter(board_state[y][x]));

            if ((x + y) % 2) {
                square->setStyleSheet("background-color: rgb(112, 86, 25);");
            }
            else square->setStyleSheet("background-color: rgb(243, 236, 220);");

            chessboard->addWidget(square, y, x);
        }
    }

    chessboard->setContentsMargins(0, 0, 0, 0);
    chessboard->setSpacing(0);
    setLayout(chessboard);
}

void Chessboard::clearBestMoveHistory()
{
    for (int i = 0; i < 4; i++) {
        past_best_move[i] = 0;
    }
}

void Chessboard::repaintChessboard()
{
    int grid_size = 8;
    for (int y = 0; y < grid_size; y++) {
        for (int x = 0; x < grid_size; x++) {

            if ((x + y) % 2) {
                chessboard->itemAtPosition(x, y)->widget()->findChild<QFrame*>()->setStyleSheet("background-color: rgb(112, 86, 25);");
            }
            else chessboard->itemAtPosition(x, y)->widget()->findChild<QFrame*>()->setStyleSheet("background-color: rgb(243, 236, 220);");

        }
    }
}

void Chessboard::resizeEvent(QResizeEvent *event) {
    int size = qMin(event->size().width(), event->size().height());
    resize(size, size);

    emit resizeEvalBar(size);
}

void Chessboard::updateChessboard(std::vector<std::vector<char>> board_state)
{
    int grid_size = 8;
    for (int y = 0; y < grid_size; y++) {
        for (int x = 0; x < grid_size; x++) {
            updateField(y, x, board_state[y][x]);
        }
    }
}

void Chessboard::resetChessboard()
{
    setBoardStateFromFEN(start_pos);
    repaintChessboard();
    clearBestMoveHistory();
    updateChessboard(board_state);
}

void Chessboard::updateField(const int X, const int Y, const char piece)
{
    QString updatedPiece = "";

    if (piece == 'p') {
        updatedPiece = ":/staunty/bP.svg";
    }
    else if (piece == 'n') {
        updatedPiece = ":/staunty/bN.svg";
    }
    else if (piece == 'b') {
        updatedPiece = ":/staunty/bB.svg";
    }
    else if (piece == 'r') {
        updatedPiece = ":/staunty/bR.svg";
    }
    else if (piece == 'q') {
        updatedPiece = ":/staunty/bQ.svg";
    }
    else if (piece == 'k') {
        updatedPiece = ":/staunty/bK.svg";
    }
    else if (piece == 'P') {
        updatedPiece = ":/staunty/wP.svg";
    }
    else if (piece == 'N') {
        updatedPiece = ":/staunty/wN.svg";
    }
    else if (piece == 'B') {
        updatedPiece = ":/staunty/wB.svg";
    }
    else if (piece == 'R') {
        updatedPiece = ":/staunty/wR.svg";
    }
    else if (piece == 'Q') {
        updatedPiece = ":/staunty/wQ.svg";
    }
    else if (piece == 'K') {
        updatedPiece = ":/staunty/wK.svg";
    }
    else {
        // hmmmm
    }

    // update field
    chessboard->itemAtPosition(X, Y)->widget()->findChild<QLabel*>()->setPixmap(QPixmap(updatedPiece));
}

void Chessboard::updateFieldBackground(const int X, const int Y, const QString style)
{
    chessboard->itemAtPosition(X, Y)->widget()->findChild<QFrame*>()->setStyleSheet(style);
}

void Chessboard::setBoardStateFromFEN(const char *FEN_string)
{
    int idx = 0;
    int i = 0;

    while (FEN_string[idx] != ' ') {
        if (isdigit(FEN_string[idx])) {
            i += FEN_string[idx] - '0';
        }
        else if (FEN_string[idx] != '/') {
            board_state[i/8][i%8] = FEN_string[idx];
            i++;
        }
        idx++;
    }
    //    print_board_state();
}

void Chessboard::print_board_state()
{
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0) qInfo() << '\n';
        qInfo() << board_state[i] << ' ';
    }
}

QString Chessboard::getPieceFromLetter(const char pieceLetter)
{
    QString piece = "";

    if (pieceLetter == 'p') {
        piece = ":/staunty/bP.svg";
    }
    else if (pieceLetter == 'n') {
        piece = ":/staunty/bN.svg";
    }
    else if (pieceLetter == 'b') {
        piece = ":/staunty/bB.svg";
    }
    else if (pieceLetter == 'r') {
        piece = ":/staunty/bR.svg";
    }
    else if (pieceLetter == 'q') {
        piece = ":/staunty/bQ.svg";
    }
    else if (pieceLetter == 'k') {
        piece = ":/staunty/bK.svg";
    }
    else if (pieceLetter == 'P') {
        piece = ":/staunty/wP.svg";
    }
    else if (pieceLetter == 'N') {
        piece = ":/staunty/wN.svg";
    }
    else if (pieceLetter == 'B') {
        piece = ":/staunty/wB.svg";
    }
    else if (pieceLetter == 'R') {
        piece = ":/staunty/wR.svg";
    }
    else if (pieceLetter == 'Q') {
        piece = ":/staunty/wQ.svg";
    }
    else if (pieceLetter == 'K') {
        piece = ":/staunty/wK.svg";
    }
    else {
        // hmmmm
    }

    return piece;
}

void Chessboard::showBestMove(QString best_move)
{
    // revert previous best move fields
    if ((past_best_move[0] + past_best_move[1]) % 2 == 0) {
        updateFieldBackground(7 - past_best_move[1], past_best_move[0], "background-color: rgb(112, 86, 25);");
    } else {
        updateFieldBackground(7 - past_best_move[1], past_best_move[0], "background-color: rgb(243, 236, 220);");
    }
    if ((past_best_move[3] + past_best_move[2]) % 2 == 0) {
        updateFieldBackground(7 - past_best_move[3], past_best_move[2], "background-color: rgb(112, 86, 25);");
    } else {
        updateFieldBackground(7 - past_best_move[3], past_best_move[2], "background-color: rgb(243, 236, 220);");
    }

    // get new best move
    int x1 = best_move[0].toLatin1() - 'a';
    int y1 = best_move[1].toLatin1() - '1';

    int x2 = best_move[2].toLatin1() - 'a';
    int y2 = best_move[3].toLatin1() - '1';

    updateFieldBackground(7 - y1, x1, "background-color: rgb(0, 255, 0);");
    updateFieldBackground(7 - y2, x2, "background-color: rgb(0, 205, 0);");

    // store current best move
    past_best_move [0] = x1;
    past_best_move [1] = y1;
    past_best_move [2] = x2;
    past_best_move [3] = y2;
}
