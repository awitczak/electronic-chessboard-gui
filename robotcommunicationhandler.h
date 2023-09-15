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

signals:
    void output(QString data);

public slots:
    void start();
    void stop();
    void send(QByteArray cmd);

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

    QProcess m_process;
    QString m_path;
    bool m_listening;
};

#endif // ROBOTCOMMUNICATIONHANDLER_H