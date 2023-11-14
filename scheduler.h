#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>
#include <QThread>
#include <QDebug>

class Scheduler : public QThread {
    Q_OBJECT

public:
    Scheduler();

    void run() override;


private:
    bool isGameFinished;
    int cnt;

};

#endif // SCHEDULER_H
