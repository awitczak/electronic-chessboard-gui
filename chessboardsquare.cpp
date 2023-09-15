#include "chessboardsquare.h"

ChessboardSquare::ChessboardSquare() : QFrame()
{
    setMinimumSize(minWidth, minHeight);

    icon = new QLabel(this);
    icon->setMinimumSize(minWidth, minHeight);
    icon->setScaledContents(true);
}

void ChessboardSquare::setIcon(const QString &svg_path)
{
    icon->setPixmap(QPixmap(svg_path));
}

void ChessboardSquare::resizeEvent(QResizeEvent *event)
{
    int size = qMin(event->size().width(), event->size().height());

    resize(size, size);
    icon->resize(size, size);
}
