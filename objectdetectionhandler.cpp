#include "objectdetectionhandler.h"


ObjectDetectionHandler::ObjectDetectionHandler(QObject *parent)
{
    connect(&m_process, &QProcess::errorOccurred, this, &ObjectDetectionHandler::errorOccurred);
    connect(&m_process, &QProcess::readyReadStandardError, this, &ObjectDetectionHandler::readyReadStandardError);
    connect(&m_process, &QProcess::readyReadStandardOutput, this, &ObjectDetectionHandler::readyReadStandardOutput);
    connect(&m_process, &QProcess::started, this, &ObjectDetectionHandler::started);
    connect(&m_process, &QProcess::stateChanged, this, &ObjectDetectionHandler::stateChanged);
    connect(&m_process, &QProcess::readyRead, this, &ObjectDetectionHandler::readyRead);

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &ObjectDetectionHandler::finished);

    m_listening = false;
    m_path = "";
}

QString ObjectDetectionHandler::getPath() const
{
    return m_path;
}

void ObjectDetectionHandler::setPath(const QString &path)
{
    m_path = path;
}

void ObjectDetectionHandler::start()
{
    m_process.start("python3", QStringList() << m_path);
    m_process.waitForStarted();
}

void ObjectDetectionHandler::stop()
{
    m_process.terminate();

//    emit object_detection_stopped();
}

void ObjectDetectionHandler::errorOccurred(QProcess::ProcessError error)
{
    qDebug() << "Error occurred: " << error;
}

void ObjectDetectionHandler::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Process finished with exit code: " << exitCode;
    qDebug() << "Exit status: " << exitStatus;
}

void ObjectDetectionHandler::readyReadStandardError()
{
    QByteArray data = m_process.readAllStandardError();
//    qDebug() << "Standard Error: " << data;

    emit output(data);

    processData(data);
}

void ObjectDetectionHandler::readyReadStandardOutput()
{
    qDebug() << "2";
}

void ObjectDetectionHandler::started()
{
    emit connected();
}

void ObjectDetectionHandler::stateChanged(QProcess::ProcessState newState)
{
//    if (newState == QProcess::Running) {
//        emit object_detection_started();
//    }
}

void ObjectDetectionHandler::readyRead()
{
    qDebug() << "5";
}

void ObjectDetectionHandler::processData(QString data)
{
    if (data.contains("person")) emit personDetected();
    else personNotDetected();
}
