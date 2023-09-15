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

    void setEvalValue(float value);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;


private:
    float currentEvalValue;
    int currentHeight;

    void setCurrentHeight(int height);
    int mapFloatToIntRange(float value, float inputMin, float inputMax, int outputMin, int outputMax);

signals:

public slots:
    void resizeEvalBar(int height);
    void updateEvalBar(QString eval, bool whoseTurn);
    void resetEvalBar();
};

#endif // EVALUATIONBAR_H