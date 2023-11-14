#include "scheduler.h"


Scheduler::Scheduler()
{
    isFinished = false;
    state = IDLE;
    qDebug() << "Scheduler initialized!";

    start();
}

void Scheduler::run()
{
    mainLoop();

    qDebug() << "Scheduler finished operation!";
}

void Scheduler::stockfishConnected()
{
    state = STOCKFISH_CONNECTED;
}

void Scheduler::mainLoop()
{
    while (!isFinished) {

        switch (state) {
        case IDLE:
            qDebug() << "IDLE!";
            state = WAIT_FOR_STOCKFISH;
            break;
        case WAIT_FOR_STOCKFISH:
            qDebug() << "WAIT_FOR_STOCKFISH!";
            break;
        case STOCKFISH_CONNECTED:
            qDebug() << "STOCKFISH_CONNECTED!";
            isFinished = true;
            break;
        }


        sleep(3);
    }
}
