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

void ChessboardSquare::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !icon->pixmap().isNull()) {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(QString(chess_piece));

        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap(":/icons/chess.png"));
        drag->exec(Qt::MoveAction);

        icon->setPixmap(QPixmap(":/icons/chess.png"));
    }

    qDebug() << "mousePressEvent";
}

void ChessboardSquare::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;

    int distance = (event->pos() - dragStartPosition).manhattanLength();
    if (distance < QApplication::startDragDistance())
        return;

    // Start the drag operation
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    mimeData->setText(QString(chess_piece)); // Set the piece type (letter) as text
    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap(":/icons/chess.png"));

    // When dragging starts, hide the current piece
    icon->hide();

    // Set the position of the drag pixmap to the center of the mouse cursor
    drag->setHotSpot(event->pos());

    // Perform the drag action
    drag->exec(Qt::MoveAction);

    qDebug() << "mouseMoveEvent";
}

void ChessboardSquare::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void ChessboardSquare::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText() && event->source() != this)
    {
        event->acceptProposedAction();
    }

    qDebug() << "dragEnterEvent";
}

void ChessboardSquare::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasText()) {
        QString pieceType = event->mimeData()->text();

        ChessboardSquare *sourceSquare = qobject_cast<ChessboardSquare*>(event->source());

        if (sourceSquare) {
            if (sourceSquare == this) {
                event->ignore();
            }
            else {
                setIcon(":/icons/chess.png");
                event->acceptProposedAction();
            }
        }
    }

    qDebug() << "dropEnterEvent";
}
