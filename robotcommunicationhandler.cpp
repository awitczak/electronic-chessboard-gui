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
    m_listening = false;
    m_process.close();
}

void RobotCommunicationHandler::send(QByteArray cmd)
{
    m_process.write(cmd + "\n");
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
}
