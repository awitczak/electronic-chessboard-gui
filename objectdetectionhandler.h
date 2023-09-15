#ifndef OBJECTDETECTIONHANDLER_H
#define OBJECTDETECTIONHANDLER_H

#include <QObject>
#include <QDebug>
#include <QProcess>
#include <QSysInfo>

class ObjectDetectionHandler : public QObject
{
    Q_OBJECT
public:
    explicit ObjectDetectionHandler(QObject *parent = nullptr);

    QString getPath() const;
    void setPath(const QString &path);

signals:
    void output(QString data);

    void object_detection_started();
    void object_detection_stopped();

public slots:
    void start();
    void stop();

private slots:
    void errorOccurred(QProcess::ProcessError error);
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void readyReadStandardError();
    void readyReadStandardOutput();
    void started();
    void stateChanged(QProcess::ProcessState newState);
    void readyRead();

private:
    QProcess m_process;
    QString m_path;
    bool m_listening;
};

#endif // OBJECTDETECTIONHANDLER_H
