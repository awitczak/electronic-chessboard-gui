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

public slots:
    void stockfishConnected();

private:

    typedef enum {
        IDLE = 0,
        WAIT_FOR_STOCKFISH,
        STOCKFISH_CONNECTED
    } State;

    State state;

    bool isFinished;

    void mainLoop();

};

#endif // SCHEDULER_H
