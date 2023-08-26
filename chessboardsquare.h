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

protected:
    void resizeEvent(QResizeEvent *event) override;

};

#endif // CHESSBOARDSQUARE_H
