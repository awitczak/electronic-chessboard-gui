#include "stockfish.h"

Stockfish::Stockfish(QObject *parent) : QObject{parent}
{
    connect(&m_process, &QProcess::errorOccurred, this, &Stockfish::errorOccurred);
    connect(&m_process, &QProcess::readyReadStandardError, this, &Stockfish::readyReadStandardError);
    connect(&m_process, &QProcess::readyReadStandardOutput, this, &Stockfish::readyReadStandardOutput);
    connect(&m_process, &QProcess::started, this, &Stockfish::started);
    connect(&m_process, &QProcess::stateChanged, this, &Stockfish::stateChanged);
    connect(&m_process, &QProcess::readyRead, this, &Stockfish::readyRead);

    connect(&m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &Stockfish::finished);

    m_listening = false;
    m_path = "";
}

QString Stockfish::getPath() const
{
    return m_path;
}

void Stockfish::setPath(const QString &path)
{
    m_path = path;
}

void Stockfish::start()
{
    m_listening = true;
    m_process.start(getProcess());
}

void Stockfish::stop()
{
    m_listening = false;
    m_process.close();

    emit disconnected();
}

void Stockfish::send(QByteArray cmd)
{
    m_process.write(cmd + "\n");
}

void Stockfish::updateFEN(QByteArray FEN)
{
    m_process.write("position fen " + FEN + "\n");

    qDebug() << "position fen " + FEN + "\n";

    // evaluate
    m_process.write("go depth 20\n");
}

void Stockfish::resetStockfish()
{
    send("ucinewgame");
    send("isready");
    send("go depth 20");

    emit connected();
}

void Stockfish::errorOccurred(QProcess::ProcessError error)
{
    if (!m_listening) return;
    emit output("Error");
}

void Stockfish::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (!m_listening) return;

    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    emit output("Complete");
}

void Stockfish::readyReadStandardError()
{
    if (!m_listening) return;

    QString msg = "Standard error: ";
    msg.append(m_process.readAllStandardError());

    emit output(msg);
}

void Stockfish::readyReadStandardOutput()
{
    if (!m_listening) return;

    QByteArray data = m_process.readAllStandardOutput();

    emit output(QString(data.trimmed()));
}

void Stockfish::started()
{
    send("uci");
    send("isready");
    send("position startpos");
    send("go depth 20");
}

void Stockfish::stateChanged(QProcess::ProcessState newState)
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

        startStockfish();

        break;
    }
}

void Stockfish::readyRead()
{
    if (!m_listening) return;

    QByteArray data = m_process.readAll().trimmed();

    emit output(data);

    getCurrentEval(data);
    getCurrentBestMove(data);
}

QString Stockfish::getProcess()
{
    if (QSysInfo::productType() == "windows") return "cmd";
    else if (QSysInfo::productType() == "osx") return "/bin/zsh";
    else return "bash";
}

void Stockfish::startStockfish()
{
    QByteArray command;
    command.append("/home/adam/Desktop/stockfish");

    if (QSysInfo::productType() == "windows") command.append("\r");
    else command.append("\n");

    m_process.write(command);

    emit connected();
}

void Stockfish::getCurrentBestMove(QString data)
{
    if (data.contains("bestmove")) {

        int bestMove_last_idx = data.indexOf("bestmove ") + strlen("bestmove ");
        int ponder_idx = data.indexOf("ponder");

        QString bestMove = data.mid(bestMove_last_idx, ponder_idx - 1 - bestMove_last_idx);

        emit currentBestMove(bestMove);


        qDebug() << "stockfish emitted ready!";
        emit ready();
    }
}

void Stockfish::getCurrentEval(QString data)
{
    static float currentEval;
    static int mateEval;

    std::string dataString = data.toStdString();
    std::string line = "";
    for (int i = 0; i < data.length(); i++) {
        if (data[i] != '\n') {
            line += dataString[i];
        }
        else {
            QString qStringLine = QString::fromStdString(line);

            int nodes_idx = qStringLine.indexOf("nodes");
            int mate_idx = qStringLine.indexOf("mate");
            int cp_idx = qStringLine.indexOf("cp");

            if (mate_idx >= 0) {
                mateEval = qStringLine.mid(mate_idx + 5, nodes_idx - mate_idx - 6).toInt();

                emit mateEvaluated(mateEval);
            }

            if (cp_idx >= 0) {
                currentEval = qStringLine.mid(cp_idx + 3, nodes_idx - cp_idx - 4).toFloat();

                emit currentEvaluation(currentEval);
            }
            line = "";
        }
    }
}
