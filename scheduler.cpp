#include "scheduler.h"


Scheduler::Scheduler()
{
    isGameFinished = false;
    cnt = 0;

    qDebug() << "Scheduler initialized!";

    start();
}

void Scheduler::run()
{
//    while (!isGameFinished) {
//        if (cnt == 10) {
//            qDebug() << "FINISHED!";
//            cnt = 0;
//            isGameFinished = true;
//        }

//        cnt++;
//    }


}
