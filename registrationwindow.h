#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QProgressBar>
#include "sessionmanager.h"


class RegistrationWindow : public QDialog
{
    Q_OBJECT
public:
    RegistrationWindow(QWidget *parent = nullptr);

private slots:


    void attemptRegistration();

private:
    QLineEdit   *loginLineEdit;
    QLineEdit   *usernameLineEdit;
    QLineEdit   *passwordLineEdit;
    QLineEdit   *confirmpasswordLineEdit;
    QLabel      *registrationErrorLabel;
    QPushButton *registerButton;

    const int minRequiredStrength = 3;


    int  calculatePasswordStrength(const QString &password);
};

#endif // REGISTRATIONWINDOW_H
