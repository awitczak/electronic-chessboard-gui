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
    void stockfishNotConnected();
    void stockfishReady();
    void stockfishBusy();
    void stockfishFault();

    void objectDetectionConnected();
    void objectDetectionDisconnected();
    void personDetected();
    void personNotDetected();

    void eChessboardConnected();
    void eChessboardDisconnected();
    void eChessboardReady();
    void eChessboardBusy();
    void eChessboardFault();
    void eChessboardNewMoveMade();

    void robotComConnected();
    void robotComDisconnected();
    void robotComReady();
    void robotComBusy();
    void robotComFault();

    void gripperConnected();
    void gripperDisconnected();
    void gripperReady();
    void gripperBusy();
    void gripperFault();

    void chessgameStart();
    void chessgameWhiteMove();
    void chessgameBlackMove();
    void chessgameEnd();


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
