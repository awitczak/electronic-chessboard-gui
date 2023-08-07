#include "inputtextbox.h"

InputTextBox::InputTextBox(QWidget *parent) : QTextEdit(parent)
{

}

void InputTextBox::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        emit enterPressed();
        clear();

        return;
    }

    QTextEdit::keyPressEvent(event);
}
