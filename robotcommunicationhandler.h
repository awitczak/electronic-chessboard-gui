#ifndef ROBOTCOMMUNICATIONHANDLER_H
#define ROBOTCOMMUNICATIONHANDLER_H

#include <QObject>
#include <QProcess>
#include <QDebug>

class RobotCommunicationHandler : public QObject
{
    Q_OBJECT
public:
    explicit RobotCommunicationHandler(QObject *parent = nullptr);

    QString getPath() const;
    void setPath(const QString &path);

    bool update_A1_pos, update_H8_pos, update_Z0_pos;

signals:
    void output(QString data);
    void moving();
    void notMoving();
    void connected();
    void disconnected();

    void tcp_updated(QString field);
    void tcp_Z0_updated();

    void getPieceFromField(QString field);

public slots:
    void start();
    void stop();
    void send(QByteArray cmd);

    void setChessboardCornerPos(QString corner);
    void setZ0();
    void setCurrentMove(QString move);

    void moveRobotToFirstField();
    void moveRobotToSecondField();

private slots:
    void errorOccurred(QProcess::ProcessError error);
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void readyReadStandardError();
    void readyReadStandardOutput();
    void started();
    void stateChanged(QProcess::ProcessState newState);
    void readyRead();

private:
    QString getProcess();
    void startRobotComm();

    void processOutput(QString data);

    QProcess m_process;
    QString m_path;
    bool m_listening;


    // robot and chessboard setup parameters
    QString fromField;
    QString toField;
    QString Z0;

    bool A1_set, H8_set;

    float chessboard_a_X;
    float chessboard_b_X;
    float chessboard_a_Y;
    float chessboard_b_Y;

    QStringList currentTCP;
    QStringList cornerPos_A1;
    QStringList cornerPos_A8;
    QStringList cornerPos_H1;
    QStringList cornerPos_H8;
    QStringList centerPos;

    void calculateRemainingCorners();
    void calculateChessboardReferenceAxes();
    void moveToPosition(QStringList targetPos);

    void moveToField(QString field);


    float chessboardSquareCenterDistance;
};

#endif // ROBOTCOMMUNICATIONHANDLER_H
