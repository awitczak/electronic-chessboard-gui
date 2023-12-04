#include "robotcommunicationhandler.h"

RobotCommunicationHandler::RobotCommunicationHandler(QObject *parent)
{
    connect(&m_process, &QProcess::errorOccurred, this, &RobotCommunicationHandler::errorOccurred);
    connect(&m_process, &QProcess::readyReadStandardError, this, &RobotCommunicationHandler::readyReadStandardError);
    connect(&m_process, &QProcess::readyReadStandardOutput, this, &RobotCommunicationHandler::readyReadStandardOutput);
    connect(&m_process, &QProcess::started, this, &RobotCommunicationHandler::started);
    connect(&m_process, &QProcess::stateChanged, this, &RobotCommunicationHandler::stateChanged);
    connect(&m_process, &QProcess::readyRead, this, &RobotCommunicationHandler::readyRead);

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &RobotCommunicationHandler::finished);

    m_listening = false;
    m_path = "";

    currentTCP = {"0", "0", "0", "0", "0", "0"};
    cornerPos_A1 = {"0", "0", "0", "0", "0", "0"};
    cornerPos_A8 = {"0", "0", "0", "0", "0", "0"};
    cornerPos_H1 = {"0", "0", "0", "0", "0", "0"};
    cornerPos_H8 = {"0", "0", "0", "0", "0", "0"};
    bucketPos = {"0", "0", "0", "0", "0", "0"};

    A1_set = false;
    H8_set = false;

    update_A1_pos = false;
    update_H8_pos = false;
    update_Z0_pos = false;

    chessboard_a_X = 0.0;
    chessboard_b_X = 0.0;
    chessboard_a_Y = 0.0;
    chessboard_b_Y = 0.0;

    chessboardSquareCenterDistance = 0.051;

    fromField = "";
    toField = "";

    Z0 = "";
    current_Z = 0;
}

QString RobotCommunicationHandler::getPath() const
{
    return m_path;
}

void RobotCommunicationHandler::setPath(const QString &path)
{
    m_path = path;
}

void RobotCommunicationHandler::start()
{
    m_listening = true;
    m_process.start(getProcess());
}

void RobotCommunicationHandler::stop()
{
    send("exit");
    m_process.terminate();
    m_listening = false;

    emit disconnected();
}

void RobotCommunicationHandler::send(QByteArray cmd)
{
    m_process.write(cmd + "\n");
}

void RobotCommunicationHandler::setChessboardCornerPos(QString corner)
{
    if (corner.contains("A1")) {
        cornerPos_A1 = currentTCP;
        A1_set = true;
    }
    if (corner.contains("H8")) {
        cornerPos_H8 = currentTCP;
        H8_set = true;
    }

    if (A1_set && H8_set) {
        calculateRemainingCorners();
        A1_set = false;
        H8_set = false;
    }

    qDebug() << currentTCP;
}

void RobotCommunicationHandler::setZ0()
{
    Z0 = currentTCP[2];

    current_Z = Z0.toFloat();

    qDebug() << "current Z0:" << Z0;
}

void RobotCommunicationHandler::setBucketPos()
{
    bucketPos = currentTCP;
}

void RobotCommunicationHandler::setCurrentMove(QString move)
{
    fromField = move.mid(0, 2).toUpper();
    toField = move.mid(2).toUpper();

    getPieceFromField(fromField);

    qDebug() << "From: " << fromField << "  to: " << toField;
}

void RobotCommunicationHandler::moveRobotToFirstField()
{
    moveToField(fromField);
}

void RobotCommunicationHandler::moveRobotToSecondField()
{
    moveToField(toField);
}

void RobotCommunicationHandler::moveRobotToBucket()
{
    moveToPosition(bucketPos);
}

void RobotCommunicationHandler::moveRobotToZ0()
{
    QString temp = "move_relative 0 0 " + QString::number(-current_Z + Z0.toFloat()) + " 0 0 0";
    current_Z = Z0.toFloat();

    send(temp.toUtf8());
}

void RobotCommunicationHandler::moveXYRelative(float X, float Y)
{
    QString temp = "move_relative " + QString::number(X) + " " + QString::number(Y) + " 0 0 0 0";

    send(temp.toUtf8());
}

void RobotCommunicationHandler::moveZRelative(float Z)
{
    current_Z += Z;
    QString temp = "move_relative 0 0 " + QString::number(Z) + " 0 0 0";

    send(temp.toUtf8());
}

void RobotCommunicationHandler::errorOccurred(QProcess::ProcessError error)
{
    if (!m_listening) return;
    emit output("Error");
}

void RobotCommunicationHandler::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (!m_listening) return;

    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    emit output("Complete");
}

void RobotCommunicationHandler::readyReadStandardError()
{
    if (!m_listening) return;

    QString msg = "Standard error: ";
    msg.append(m_process.readAllStandardError());

    emit output(msg);
}

void RobotCommunicationHandler::readyReadStandardOutput()
{
    if (!m_listening) return;

    QByteArray data = m_process.readAllStandardOutput();

    emit output(QString(data.trimmed()));
}

void RobotCommunicationHandler::started()
{
    send("Robot connection established");
    send("get_tcp_pos");
}

void RobotCommunicationHandler::stateChanged(QProcess::ProcessState newState)
{
    switch (newState) {
    case QProcess::NotRunning:
        emit output ("Not running");
        break;
    case QProcess::Starting:
        emit output ("Starting...");
        break;
    case QProcess::Running:
        emit output ("Running");

        startRobotComm();

        break;
    }
}

void RobotCommunicationHandler::readyRead()
{
    if (!m_listening) return;

    QByteArray data = m_process.readAll().trimmed();

    processOutput(data);

    emit output(data);
}

QString RobotCommunicationHandler::getProcess()
{
    if (QSysInfo::productType() == "windows") return "cmd";
    else if (QSysInfo::productType() == "osx") return "/bin/zsh";
    else return "bash";
}

void RobotCommunicationHandler::startRobotComm()
{
    QByteArray command;
    command.append(m_path.toUtf8());

    if (QSysInfo::productType() == "windows") command.append("\r");
    else command.append("\n");

    m_process.write(command);

    emit connected();
}

void RobotCommunicationHandler::processOutput(QString data)
{
    if (data.contains("Moving to")) {
        emit moving();
    }
    if (data.contains("Target reached!")) {
        emit notMoving();
    }

    if (data.contains("current_tcp_pos")) {

        int temp_idx = data.indexOf("current_tcp_pos: ");
        QString temp = data.mid(temp_idx + strlen("current_tcp_pos: "));

        currentTCP = temp.split(" ");

        if (update_A1_pos) {
            emit tcp_updated("A1");
            update_A1_pos = false;
        }
        if (update_H8_pos) {
            emit tcp_updated("H8");
            update_H8_pos = false;
        }
        if (update_Z0_pos) {
            emit tcp_Z0_updated();
            update_Z0_pos = false;
        }
        if (update_bucket_pos) {
            emit bucket_tcp_updated();
            update_bucket_pos = false;
        }
    }
}

void RobotCommunicationHandler::calculateRemainingCorners()
{
    float x_center = 0.0;
    float y_center = 0.0;

    float x_A1 = cornerPos_A1[0].toFloat();
    float y_A1 = cornerPos_A1[1].toFloat();

    float x_H8 = cornerPos_H8[0].toFloat();
    float y_H8 = cornerPos_H8[1].toFloat();

    x_center = (x_A1 + x_H8) / 2;
    y_center = (y_A1 + y_H8) / 2;

    qDebug() << "x/y_center: " << x_center << y_center;

    float v_x = x_A1 - x_center;
    float v_y = y_A1 - y_center;

    cornerPos_A8 = cornerPos_A1;
    cornerPos_A8[0] = QString::number(x_center + v_y);
    cornerPos_A8[1] = QString::number(y_center - v_x);

    cornerPos_H1 = cornerPos_H8;
    cornerPos_H1[0] = QString::number(x_center - v_y);
    cornerPos_H1[1] = QString::number(y_center + v_x);

    calculateChessboardReferenceAxes();
}

void RobotCommunicationHandler::calculateChessboardReferenceAxes()
{
    // x axis
    float x_A1 = cornerPos_A1[0].toFloat();
    float y_A1 = cornerPos_A1[1].toFloat();

    float x_H1 = cornerPos_H1[0].toFloat();
    float y_H1 = cornerPos_H1[1].toFloat();

    chessboard_a_X = (y_H1 - y_A1) / (x_H1 - x_A1);
    chessboard_b_X = y_A1 - chessboard_a_X * x_A1;

    float x_A8 = cornerPos_A8[0].toFloat();
    float y_A8 = cornerPos_A8[1].toFloat();

    chessboard_a_Y = (y_A8 - y_A1) / (x_A8 - x_A1);
    chessboard_b_Y = y_A1 - chessboard_a_Y * x_A1;
}

void RobotCommunicationHandler::moveToPosition(QStringList targetPos)
{
    QString move_cmd = "move_absolute ";

    targetPos[2] = QString::number(current_Z);

    for (auto p : targetPos) move_cmd += p + " ";

    qDebug() << move_cmd;

    send(move_cmd.toUtf8());
}

void RobotCommunicationHandler::moveToField(QString field)
{
    int x = field[0].unicode() - 'A';
    int y = field[1].unicode() - '0' - 1;

    qDebug() << x << y;

    QStringList targetPos = cornerPos_A1;

    // needs a rework, now hard coded X/Y coords, needs to account for rotation of new coordinate frame regarding the robot coordinate frame

    targetPos[0] = QString::number(targetPos[0].toFloat() + chessboardSquareCenterDistance * x);
    targetPos[1] = QString::number(targetPos[1].toFloat() + chessboardSquareCenterDistance * y);

    moveToPosition(targetPos);
}
