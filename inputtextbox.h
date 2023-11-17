#ifndef INPUTTEXTBOX_H
#define INPUTTEXTBOX_H

#include <QTextEdit>
#include <QKeyEvent>

class InputTextBox : public QTextEdit
{
    Q_OBJECT
public:
    InputTextBox(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void enterPressed(const QByteArray &data);
};

#endif // INPUTTEXTBOX_H
