#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QCheckBox>
#include <QSettings>


class LoginWindow : public QDialog
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);

private:
    QLineEdit *loginLineEdit, *passwordLineEdit;
    QPushButton *loginButton,*registrButton;
    QToolButton *showPasswordButton;
    QLabel *loginErrorLabel;
    QCheckBox *rememberMeCheckBox;

private slots:

    void attemptLogin();
    void openRegistrationWindow();

};

#endif // LOGINWINDOW_H
