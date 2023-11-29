#include "scheduler.h"


Scheduler::Scheduler()
{
    f_scheduler.isFinished = false;
    f_scheduler.stateMsg = false;

    f_stockfish.connected = false;
    f_stockfish.ready = false;

    f_eChessboard.connected = false;
    f_eChessboard.setupDone = false;
    f_eChessboard.correctPos = false;
    f_eChessboard.movePlayed = false;

    f_chessgame.started = false;
    f_chessgame.finished = false;
    f_chessgame.turn = false;

    f_detection.connected = false;
    f_detection.person = false;

    f_gripper.connected = false;
    f_gripper.ready = false;

    f_robotCom.connected = false;
    f_robotCom.moving = false;
    f_robotCom.interrupted = false;

    state = SETUP;
    schedulerMsg("Scheduler initialized!");

    pieceToMove = '0';

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
    f_stockfish.ready = true;
}

void Scheduler::stockfishBusy()
{
    f_stockfish.ready = false;
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
        f_robotCom.moving = false;
    }
}

void Scheduler::personNotDetected()
{
    if (f_detection.person) {
        msleep(5000);
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

void Scheduler::eChessboardInitBoardSetupDone()
{
    f_eChessboard.setupDone = true;
}

void Scheduler::eChessboardInitBoardSetupWait()
{
    f_eChessboard.setupDone = false;
}

void Scheduler::eChessboardReturnToPositionWait()
{
    f_eChessboard.correctPos = false;
}

void Scheduler::eChessboardReturnToPositionDone()
{
    f_eChessboard.correctPos = true;
    schedulerMsg("correct");
}

void Scheduler::eChessboardMovePlayed()
{
    f_eChessboard.movePlayed = true;
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

void Scheduler::robotComMoving()
{
    schedulerMsg("robotCom started movement!");
    f_robotCom.moving = true;
}

void Scheduler::robotComNotMoving()
{
    schedulerMsg("robotCom finished movement!");
    f_robotCom.moving = false;
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
    f_gripper.ready = true;
}

void Scheduler::gripperBusy()
{
    f_gripper.ready = false;
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

void Scheduler::chessgamePieceInfo(char piece)
{
    pieceToMove = piece;

    qDebug() << "scheduler piece to move: " << pieceToMove;
}

void Scheduler::chessgameEnd()
{

}

void Scheduler::reset()
{
    quit();

    f_scheduler.isFinished = false;
    f_scheduler.stateMsg = false;

    f_stockfish.connected = false;
    f_stockfish.ready = false;

    f_eChessboard.connected = false;
    f_eChessboard.setupDone = false;
    f_eChessboard.correctPos = false;
    f_eChessboard.movePlayed = false;

    f_chessgame.started = false;
    f_chessgame.finished = false;
    f_chessgame.turn = false;

    f_detection.connected = false;
    f_detection.person = false;

    f_gripper.connected = false;
    f_gripper.ready = false;

    f_robotCom.connected = false;
    f_robotCom.moving = false;
    f_robotCom.interrupted = false;

    state = SETUP;
    schedulerMsg("Scheduler initialized!");

    start();
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

            qDebug() << f_stockfish.connected << f_eChessboard.connected << f_detection.connected << f_gripper.connected << f_robotCom.connected;

            if (f_stockfish.connected && f_eChessboard.connected && f_detection.connected && f_gripper.connected && f_robotCom.connected) {
                schedulerMsg("Setup done!");
                state = GAME;
                f_scheduler.stateMsg = false;
            }

            state = GAME;
            break;

        case GAME:
            if (!f_scheduler.stateMsg) {
                schedulerMsg("GAME!");
                f_scheduler.stateMsg = true;
            }

//            if (!f_eChessboard.connected) {
//                state = FAULT;
//                break;
//            }

            if (f_eChessboard.setupDone) {
                {
                    f_eChessboard.correctPos = true;
                    f_chessgame.started = true;
                    while (f_chessgame.started) {
                        if (f_eChessboard.correctPos) {
                            if (f_eChessboard.movePlayed) {
                                f_chessgame.turn = !f_chessgame.turn;

                                if (f_chessgame.turn) {

                                    while (!f_stockfish.ready) {
                                        schedulerMsg("Robot thinking!");
                                        msleep(500);
                                    }

                                    schedulerMsg("Black moving!");

                                    emit sendGripperCommand("home");
                                    msleep(1000);
                                    while (!f_gripper.ready) {
                                        // wait
                                        // schedulerMsg("waiting for gripper");
                                        msleep(50);
                                    }

                                    emit moveRobotToFirstField();
                                    msleep(500);
                                    while (f_robotCom.moving) {
                                        // wait
                                        // schedulerMsg("waiting for robot");
                                        msleep(250);
                                    }

                                    emit sendGripperCommand("open");
                                    msleep(1000);
                                    while (!f_gripper.ready) {
                                        // wait
                                        // schedulerMsg("waiting for gripper");
                                        msleep(50);
                                    }

                                    emit moveRobotToZ0();
                                    msleep(500);
                                    while (f_robotCom.moving) {
                                        // wait
                                        // schedulerMsg("waiting for robot");
                                        msleep(250);
                                    }

                                    emit sendGripperCommand(getPieceFromLetter(pieceToMove));
                                    msleep(1000);
                                    while (!f_gripper.ready) {
                                        // wait
                                        // schedulerMsg("waiting for gripper");
                                        msleep(50);
                                    }

                                    emit moveRobotToZ(0.12);
                                    msleep(500);
                                    while (f_robotCom.moving) {
                                        // wait
                                        // schedulerMsg("waiting for robot");
                                        msleep(250);
                                    }

                                    emit moveRobotToSecondField();
                                    msleep(500);
                                    while (f_robotCom.moving) {
                                        // wait
                                        // schedulerMsg("waiting for robot");
                                        msleep(250);
                                    }

                                    emit moveRobotToZ0();
                                    msleep(1000);
                                    while (f_robotCom.moving) {
                                        // wait
                                        // schedulerMsg("waiting for robot");
                                        msleep(250);
                                    }

                                    msleep(250);
                                    emit sendGripperCommand("open");
                                    msleep(1000);
                                    while (!f_gripper.ready) {
                                        // wait
                                        // schedulerMsg("waiting for gripper");
                                        msleep(50);
                                    }

                                    msleep(500);
                                    emit moveRobotToZ(0.12);
                                    msleep(500);
                                    while (f_robotCom.moving) {
                                        // wait
                                        // schedulerMsg("waiting for robot");
                                        msleep(250);
                                    }


                                    f_eChessboard.movePlayed = false;
                                }
                                else {
                                    schedulerMsg("White moving!");

                                    f_eChessboard.movePlayed = false;
                                }
                            }
                            else {
                                // schedulerMsg("Waiting for a move!");
                            }
                            f_stockfish.ready = false;
                        }
                        else {
                            schedulerMsg("Error detected!");
                            // signal to emit an error to screen, that the position needs to be adjusted

                            while (!f_eChessboard.correctPos) {
                                msleep(500);
                            }
                            schedulerMsg("Resumed!");
                        }

                        msleep(500);
                    }
                }

            }



            break;

        case FINISHED:

            break;

        case FAULT:
            // handle faults

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

QString Scheduler::getPieceFromLetter(const char pieceLetter)
{
    QString piece = "";

    QChar p = QChar::fromLatin1(pieceLetter);
    p = p.toLower();

    if (p == 'p') {
        piece = "pawn";
    }
    else if (p == 'n') {
        piece = "knight";
    }
    else if (p == 'b') {
        piece = "bishop";
    }
    else if (p == 'r') {
        piece = "rook";
    }
    else if (p == 'q') {
        piece = "queen";
    }
    else if (p == 'k') {
        piece = "king";
    }

    return piece;
}
