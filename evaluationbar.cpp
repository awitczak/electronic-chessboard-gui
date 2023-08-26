#include "evaluationbar.h"

EvaluationBar::EvaluationBar(QWidget *parent)
    : QWidget{parent}
{
    setMaximumWidth(30);
    setMinimumSize(30, 800);
    setEvalValue(0.0);
}

void EvaluationBar::setEvalValue(float value)
{
    currentEvalValue = value;
    update();
}

void EvaluationBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    int value = mapFloatToIntRange(currentEvalValue, -50.0, 50.0, 0, 100);

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
        painter.setFont(QFont("Arial", 8));
        QString evalText = QString::number(currentEvalValue, 'f', 2);
        painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignBottom, evalText);
    }
    else {
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 8));
        QString evalText = QString::number(currentEvalValue, 'f', 2);
        painter.drawText(rect(), Qt::AlignHCenter | Qt::AlignTop, evalText);
    }
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

void EvaluationBar::resizeEvalBar(int height)
{
    this->setMaximumHeight(height);
}

void EvaluationBar::updateEvalBar(QString eval, bool whoseTurn)
{
    std::string dataString = eval.toStdString();

    static float newEvalValue;

    std::string line = "";
    for (int i = 0; i < eval.length(); i++) {
        if (eval[i] != '\n') {
            line += dataString[i];
        }
        else {
            QString qStringLine = QString::fromStdString(line);

            int nodes_idx = qStringLine.indexOf("nodes");
            int mate_idx = qStringLine.indexOf("mate");
            int cp_idx = qStringLine.indexOf("cp");

            if (mate_idx >= 0) {

            }

            if (cp_idx >= 0) {
                newEvalValue = qStringLine.mid(cp_idx + 3, nodes_idx - cp_idx - 4).toInt();
            }
            line = "";
        }
    }

    if (!whoseTurn) {
        setEvalValue(newEvalValue/100 * -1);
    }
    else {
        setEvalValue(newEvalValue/100);
    }
}

void EvaluationBar::resetEvalBar()
{
    setEvalValue(0.0);
}
