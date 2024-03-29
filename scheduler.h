#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QObject>
#include <QThread>
#include <QDebug>

typedef enum {
    IDLE = 0,
    SETUP,
    GAME,
    FINISHED,
    FAULT

} State;

typedef struct {
    bool stateMsg;
    bool isFinished;
} schedulerFlags;

typedef struct {
    bool connected;
    bool ready;

} stockfishFlags;

typedef struct {
    bool connected;
    bool setupDone;
    bool correctPos;
    bool movePlayed;

} eChessboardFlags;

typedef struct {
    bool started;
    bool finished;
    bool turn;
    bool normal;
    bool capture;
    bool shortCastle;
    bool longCastle;
    bool promotion;

} chessgameFlags;

typedef struct {
    bool connected;
    bool person;

} objectDetectionFlags;

typedef struct {
    bool connected;
    bool ready;

} gripperFlags;

typedef struct {
    bool connected;
    bool moving;
    bool interrupted;

} robotCommunicationFlags;

class Scheduler : public QThread {
    Q_OBJECT

public:
    Scheduler();

    void run() override;

signals:
    void sendRobotCommand(QByteArray);
    void sendGripperCommand(QString);

    void moveRobotToFirstField();
    void moveRobotToSecondField();
    void moveRobotToZ0();
    void moveRobotToBucket();
    void moveRobotToZ(float Z);
    void moveRobotToXY(float X, float Z);

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
    void eChessboardInitBoardSetupDone();
    void eChessboardInitBoardSetupWait();
    void eChessboardReturnToPositionWait();
    void eChessboardReturnToPositionDone();
    void eChessboardMovePlayed();

    void robotComConnected();
    void robotComDisconnected();
    void robotComReady();
    void robotComBusy();
    void robotComMoving();
    void robotComNotMoving();
    void robotComFault();

    void gripperConnected();
    void gripperDisconnected();
    void gripperReady();
    void gripperBusy();
    void gripperFault();

    void chessgameStart();
    void chessgameWhiteMove();
    void chessgameBlackMove();
    void chessgamePieceInfo(char piece);
    void chessgameNormalMove();
    void chessgameCaptureMove();
    void chessgameShortCastleMove();
    void chessgameLongCastleMove();
    void chessgamePromotionMove();
    void chessgameEnd();

    void reset();


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
    QString getPieceFromLetter(const char pieceLetter);

    char pieceToMove;

    void sendGripperCmd(QString cmd);

};

#endif // SCHEDULER_H
