#ifndef STOCKFISH_H
#define STOCKFISH_H

#include <QObject>
#include <QDebug>
#include <QProcess>
#include <QSysInfo>

class Stockfish : public QObject
{
    Q_OBJECT
public:
    explicit Stockfish(QObject *parent = nullptr);

    QString getPath() const;
    void setPath(const QString &path);

signals:
    void connected();
    void disconnected();
    void output(QString data);
    void currentEvaluation(float eval);
    void mateEvaluated(int mate_N);
    void currentBestMove(QString bestMove);
    void ready();

public slots:
    void start();
    void stop();
    void send(QByteArray cmd);
    void updateFEN(QByteArray FEN);
    void resetStockfish();

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

    QString getProcess();
    void startStockfish();

    void getCurrentBestMove(QString data);
    void getCurrentEval(QString data);
};

#endif // STOCKFISH_H
