#include "evaluationbar.h"

EvaluationBar::EvaluationBar(QWidget *parent)
    : QWidget{parent}
{
    setFixedWidth(30);
    setMinimumHeight(800);
    setEvalValue(0.0);

    mateFound = false;
    whiteTurn = true;
    blackTurn = false;
}

void EvaluationBar::setEvalValue(float eval)
{
    currentEvalValue = eval;
}

void EvaluationBar::flipTurns()
{
    whiteTurn = !whiteTurn;
    blackTurn = !blackTurn;
}

void EvaluationBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    int value = mapFloatToIntRange(currentEvalValue, -50.0, 50.0, 0, 100);

    qDebug() << value << mateFound;

    if (mateFound) {
        if (value >= 0) {
            value = 100;
        }
        else {
            value = 0;
        }
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(82, 56, 5));
    painter.drawRect(rect());

    painter.setBrush(QColor(253, 246, 230));
    int progressHeight = height() * value / 100;
    painter.drawRect(0, height() - progressHeight, width(), progressHeight);

    if (currentEvalValue >= 0.0) {
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 9));
        QString evalText = QString::number(currentEvalValue, 'f', 2);
        if (mateFound) evalText = "M" + QString::number(currentMateN);
        painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignBottom, evalText);
    }
    else {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 9));
        QString evalText = QString::number(currentEvalValue, 'f', 2);
        if (mateFound) evalText = "M" + QString::number(currentMateN);
        painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignTop, evalText);
    }
}

void EvaluationBar::resizeEvent(QResizeEvent *event)
{
    resize(30, currentHeight);
}

void EvaluationBar::setCurrentHeight(int height)
{
    currentHeight = height;
}

int EvaluationBar::mapFloatToIntRange(float value, float inputMin, float inputMax, int outputMin, int outputMax)
{
    value = std::min(std::max(value, inputMin), inputMax); // ensure input in the given range

    float inputRange = inputMax - inputMin;
    int outputRange = outputMax - outputMin;
    float relativePosition = (value - inputMin) / inputRange;

    // linear mapping
    // int outputValue = outputMin + static_cast<int>(relativePosition * outputRange);

    const float sensitivityFactor = 0.4;

    float mappingFactor = 0.0;
    if (relativePosition >= 0.5) {
        mappingFactor = std::pow(relativePosition - 0.5, sensitivityFactor) + 0.5;
    }
    else {
        mappingFactor = -std::pow(0.5 - relativePosition, sensitivityFactor) + 0.5;
    }

    int outputValue = outputMin + static_cast<int>(mappingFactor * outputRange);

    return outputValue;
}

void EvaluationBar::mateEvaluated(int mate_N)
{
    mateFound = true;
    currentMateN = mate_N;

    if (whiteTurn) {
        setEvalValue(100);
    }

    if (blackTurn) {
        setEvalValue(-100);
    }

    update();
}

void EvaluationBar::resizeEvalBar(int height)
{
    setCurrentHeight(height);
}

void EvaluationBar::updateEvalBar(float eval)
{
    mateFound = false;

    if (whiteTurn) {
        setEvalValue(eval/100);
    }

    if (blackTurn) {
        setEvalValue(eval/100 * -1);
    }

    update();
}

void EvaluationBar::resetEvalBar()
{
    setEvalValue(0.0);
    mateFound = false;
}
