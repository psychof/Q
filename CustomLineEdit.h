#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QMouseEvent>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit CustomLineEdit(QWidget *parent = nullptr);

private slots:
    void showPassword();
    void hidePassword();

private:
    QToolButton *visibilityButton;
};

#endif // CUSTOMLINEEDIT_H


