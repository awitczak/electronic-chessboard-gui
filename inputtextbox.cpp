#include "inputtextbox.h"

InputTextBox::InputTextBox(QWidget *parent) : QTextEdit(parent)
{

}

void InputTextBox::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Enter:
        emit enterPressed(toPlainText().toLocal8Bit());

        clear();

        return;

        break;
    case Qt::Key_Return:
        emit enterPressed(toPlainText().toLocal8Bit());

        clear();

        return;

        break;
    default:
        break;
    }

    QTextEdit::keyPressEvent(event);
}
