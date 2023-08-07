#include "chessboard.h"

Chessboard::Chessboard(QWidget *parent) : QWidget(parent)
{
    createChessboard();
}

void Chessboard::createChessboard()
{
    setBoardStateFromFEN(start_pos);

    chessboard = new QGridLayout(this);

    int idx = 0;
    int grid_size = 8;
    for (int y = 0; y < grid_size; y++) {
        for (int x = 0; x < grid_size; x++) {

            ChessboardSquare *square = new ChessboardSquare();

            square->setIcon(getPieceFromLetter(board_state[idx]));

            if ((x + y) % 2) {
                square->setStyleSheet("background-color: rgb(112, 86, 25);");
            }
            else square->setStyleSheet("background-color: rgb(243, 236, 220);");

            chessboard->addWidget(square, y, x);

            idx++;
        }
    }

    chessboard->setSpacing(0);
    setLayout(chessboard);
    setMinimumSize(800, 800);
}

void Chessboard::resizeEvent(QResizeEvent *event) {
    int size = qMin(event->size().width(), event->size().height());
    resize(size, size);



    emit resizeEvalBar(size);
}

void Chessboard::updateChessboard()
{
    int idx = 0;
    int grid_size = 8;
    for (int y = 0; y < grid_size; y++) {
        for (int x = 0; x < grid_size; x++) {
            updateField(y, x, board_state[idx]);
            idx++;
        }
    }
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

void Chessboard::setBoardStateFromFEN(const char *FEN_string)
{
    for (int i = 0; i < N_squares; i++) {
        board_state[i] = '.';
    }

    int idx = 0;
    const char* fen = FEN_string;

    while (*fen != ' ') {
        if (isdigit(*fen)) {
            idx += *fen - '0';
        }
        else if (*fen != '/') {
            board_state[idx] = *fen;
            idx++;
        }

        fen++;
    }

    //    print_board_state();
}

void Chessboard::print_board_state()
{
    for (int i = 0; i < N_squares; i++) {
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
