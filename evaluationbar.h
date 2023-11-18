#ifndef EVALUATIONBAR_H
#define EVALUATIONBAR_H

#include <QWidget>
#include <QPainter>
#include <QResizeEvent>

class EvaluationBar : public QWidget
{
    Q_OBJECT
public:
    explicit EvaluationBar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;


private:
    float currentEvalValue;
    int currentMateN;
    int currentHeight;

    bool mateFound;
    bool whiteTurn;
    bool blackTurn;

    void setCurrentHeight(int height);
    int mapFloatToIntRange(float value, float inputMin, float inputMax, int outputMin, int outputMax);

signals:

public slots:
    void mateEvaluated(int mate_N);
    void setEvalValue(float eval);
    void flipTurns();

    void resizeEvalBar(int height);
    void updateEvalBar(float eval);
    void resetEvalBar();
};

#endif // EVALUATIONBAR_H
