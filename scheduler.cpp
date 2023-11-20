#include "scheduler.h"


Scheduler::Scheduler()
{
    f_scheduler.isFinished = false;
    f_scheduler.stateMsg = false;

    f_stockfish.connected = false;

    f_eChessboard.connected = false;

    f_detection.connected = false;
    f_detection.person = false;

    f_gripper.connected = false;

    f_robotCom.connected = false;
    f_robotCom.moving = false;
    f_robotCom.interrupted = false;

    state = SETUP;
    schedulerMsg("Scheduler initialized!");

    start();
}

void Scheduler::run()
{
    mainLoop();
}

void Scheduler::stockfishConnected()
{
    schedulerMsg("Stockfish connected!");
    f_stockfish.connected = true;
}

void Scheduler::stockfishDisconnected()
{
    schedulerMsg("Stockfish disconnected!");
    f_stockfish.connected = false;
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
    schedulerMsg("Object detection script connected!");
    f_detection.connected = true;
}

void Scheduler::objectDetectionDisconnected()
{
    schedulerMsg("Object detection script disconnected!");
    f_detection.connected = false;
}

void Scheduler::personDetected()
{
    if (!f_detection.person && f_robotCom.moving) {
        f_detection.person = true;
        f_robotCom.interrupted = true;
        emit sendRobotCommand("stop");

        schedulerMsg("Person detected - stop initiated.");

        msleep(2500);
        f_robotCom.moving = false;
    }
}

void Scheduler::personNotDetected()
{
    if (f_detection.person) {
        msleep(1500);
        f_detection.person = false;

        if (f_robotCom.interrupted) {
            emit sendRobotCommand("home");

            schedulerMsg("Robot returning home after interruption.");
        }
    }
}

void Scheduler::eChessboardConnected()
{
    schedulerMsg("eChessboard connected!");
    f_eChessboard.connected = true;
}

void Scheduler::eChessboardDisconnected()
{
    schedulerMsg("eChessboard connected!");
    f_eChessboard.connected = false;
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
    schedulerMsg("robotCom connected!");
    f_robotCom.connected = true;
}

void Scheduler::robotComDisconnected()
{
    schedulerMsg("robotCom disconnected!");
    f_robotCom.connected = false;
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
    schedulerMsg("Gripper connected!");
    f_gripper.connected = true;
}

void Scheduler::gripperDisconnected()
{
    schedulerMsg("Gripper disconnected!");
    f_gripper.connected = false;
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
    while (!f_scheduler.isFinished) {

        switch (state) {
        case IDLE:
            if (!f_scheduler.stateMsg) {
                schedulerMsg("IDLE!");
                f_scheduler.stateMsg = true;
            }

            // do nothing
            break;
        case SETUP:
            if (!f_scheduler.stateMsg) {
                schedulerMsg("SETUP!");
                f_scheduler.stateMsg = true;
            }

            if (f_stockfish.connected && f_eChessboard.connected && f_detection.connected && f_gripper.connected && f_robotCom.connected) {
                schedulerMsg("Setup done!");
                state = GAME;
            }

        case GAME:
            if (!f_scheduler.stateMsg) {
                schedulerMsg("GAME!");
                f_scheduler.stateMsg = true;
            }

            break;

        case FINISHED:
            break;

        default:

            break;
        }


        msleep(250);
    }

    schedulerMsg("Scheduler finished!");
}

void Scheduler::schedulerMsg(QString msg)
{
    qDebug() << "SCH>> " << msg;
}
