#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>
#include <QThread>
#include <QDebug>

typedef enum {
    IDLE = 0,
    SETUP,
    GAME,
    FINISHED

} State;

typedef struct {
    bool stateMsg;
    bool isFinished;
} schedulerFlags;

typedef struct {
    bool connected;

} stockfishFlags;

typedef struct {
    bool connected;

} eChessboardFlags;

typedef struct {
    bool started;
    bool finished;

} chessgameFlags;

typedef struct {
    bool connected;

} objectDetectionFlags;

typedef struct {
    bool connected;

} gripperFlags;

typedef struct {
    bool connected;

} robotCommunicationFlags;

class Scheduler : public QThread {
    Q_OBJECT

public:
    Scheduler();

    void run() override;

signals:
    void sendRobotCommand(QString);
    void sendGripperCommand(QString);

public slots:
    void stockfishConnected();
    void stockfishDisconnected();
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

    State state;

    schedulerFlags f_scheduler;
    stockfishFlags f_stockfish;
    eChessboardFlags f_eChessboard;
    chessgameFlags f_chessgame;
    objectDetectionFlags f_detection;
    gripperFlags f_gripper;
    robotCommunicationFlags f_robotCom;

    void mainLoop();
    void initFlags();
    void schedulerMsg(QString msg);

};

#endif // SCHEDULER_H
