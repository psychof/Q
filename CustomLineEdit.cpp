#include "CustomLineEdit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent) : QLineEdit(parent)
{
    visibilityButton = new QToolButton(this);
    visibilityButton->setIcon(QIcon(""));
    visibilityButton->setCursor(Qt::PointingHandCursor);
    visibilityButton->setStyleSheet(
        "QToolButton {"
        "    background-color: transparent;"
        "    border: 2px solid black;" // Измените цвет и толщину бортиков
        "    border-radius: 4px;"      // Закругленные углы
        "}"
        );
    visibilityButton->setChecked(true);

    QHBoxLayout *passwordLayout = new QHBoxLayout(this);
    passwordLayout->addWidget(visibilityButton, 0, Qt::AlignRight);
    passwordLayout->setContentsMargins(0, 0, 0, 0);
    passwordLayout->setSpacing(0);

    connect(visibilityButton, &QToolButton::pressed, this, &CustomLineEdit::showPassword);
    connect(visibilityButton, &QToolButton::released, this, &CustomLineEdit::hidePassword);
    connect(this, &QLineEdit::textChanged, this, &CustomLineEdit::hidePassword);
}

void CustomLineEdit::showPassword()
{
    setEchoMode(QLineEdit::Normal);
    visibilityButton->setIcon(QIcon(""));
}

void CustomLineEdit::hidePassword()
{
    if (!visibilityButton->isChecked())
    {
        setEchoMode(QLineEdit::Password);
        visibilityButton->setIcon(QIcon(""));
    }
}
