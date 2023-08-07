#ifndef KEYENTERRECEIVER_H
#define KEYENTERRECEIVER_H

#include <QObject>
#include <QKeyEvent>
#include <QEvent>
#include <QDebug>
#include <QWidget>

class keyEnterReceiver : public QObject
{
    Q_OBJECT
public:
    explicit keyEnterReceiver(QObject *parent = nullptr);

protected:
    bool evenFilter(QObject* obj, QEvent* event);

signals:

};

#endif // KEYENTERRECEIVER_H
