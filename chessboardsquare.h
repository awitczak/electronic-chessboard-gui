#ifndef CHESSBOARDSQUARE_H
#define CHESSBOARDSQUARE_H

#include <QResizeEvent>
#include <QFrame>
#include <QLabel>
#include <QDrag>
#include <QMimeData>
#include <QApplication>

class ChessboardSquare : public QFrame
{
    Q_OBJECT
public:
    ChessboardSquare();

    void setIcon(const QString &svg_path);

private:
    int minWidth = 100, minHeight = 100;
    QLabel *icon;
    QPoint dragStartPosition;
    char chess_piece;

protected:
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

#endif // CHESSBOARDSQUARE_H
