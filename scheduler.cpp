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
    qDebug() << "Stockfish connected!";
}

void Scheduler::stockfishNotConnected()
{

}

void Scheduler::stockfishReady()
{

}

void Scheduler::stockfishBusy()
{

}

void Scheduler::stockfishFault()
{

}

void Scheduler::objectDetectionConnected()
{
    qDebug() << "Object detection script connected!";
}

void Scheduler::objectDetectionDisconnected()
{

}

void Scheduler::personDetected()
{
//    qDebug() << "Person detected!";
}

void Scheduler::personNotDetected()
{

}

void Scheduler::eChessboardConnected()
{

}

void Scheduler::eChessboardDisconnected()
{

}

void Scheduler::eChessboardReady()
{

}

void Scheduler::eChessboardBusy()
{

}

void Scheduler::eChessboardFault()
{

}

void Scheduler::eChessboardNewMoveMade()
{

}

void Scheduler::robotComConnected()
{

}

void Scheduler::robotComDisconnected()
{

}

void Scheduler::robotComReady()
{

}

void Scheduler::robotComBusy()
{

}

void Scheduler::robotComFault()
{

}

void Scheduler::gripperConnected()
{

}

void Scheduler::gripperDisconnected()
{

}

void Scheduler::gripperReady()
{

}

void Scheduler::gripperBusy()
{

}

void Scheduler::gripperFault()
{

}

void Scheduler::chessgameStart()
{

}

void Scheduler::chessgameWhiteMove()
{

}

void Scheduler::chessgameBlackMove()
{

}

void Scheduler::chessgameEnd()
{

}

void Scheduler::mainLoop()
{
    while (!isFinished) {

//        switch (state) {
//        case IDLE:
//            qDebug() << "IDLE!";
//            state = WAIT_FOR_STOCKFISH;
//            break;
//        case WAIT_FOR_STOCKFISH:
//            qDebug() << "WAIT_FOR_STOCKFISH!";
//            break;
//        case STOCKFISH_CONNECTED:
//            qDebug() << "STOCKFISH_CONNECTED!";
//            isFinished = true;
//            break;
//        }


        sleep(3);
    }
}
