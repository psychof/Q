#include "registrationwindow.h"
#include "qregularexpression.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "database.h"


RegistrationWindow::RegistrationWindow(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Регистрация");

    QVBoxLayout *layout = new QVBoxLayout(this);

    loginLineEdit = new QLineEdit(this);
    loginLineEdit->setPlaceholderText("Логин");
    layout->addWidget(loginLineEdit);

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("Пароль");
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    connect(passwordLineEdit, &QLineEdit::textChanged, this, &::RegistrationWindow::calculatePasswordStrength);
    layout->addWidget(passwordLineEdit);

    confirmpasswordLineEdit = new QLineEdit(this);
    confirmpasswordLineEdit->setPlaceholderText("Подтвердите Пароль");
    confirmpasswordLineEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(confirmpasswordLineEdit);

    registrationErrorLabel = new QLabel(this);
    registrationErrorLabel->setStyleSheet("color:red;");
    layout->addWidget(registrationErrorLabel);

    registerButton = new QPushButton("Зарегистрироватся",this);
    connect(registerButton,&QPushButton::clicked,this,&RegistrationWindow::attemptRegistration);
    layout->addWidget(registerButton);
}


int RegistrationWindow::calculatePasswordStrength(const QString &password){

    int strength = 0;

    if (password.length() >= 8)
        strength++;

    QRegularExpression digitRegex("\\d");
    QRegularExpression lowerRegex("[a-z]");
    QRegularExpression upperRegex("[A-Z]");
    QRegularExpression specialRegex("[!@#$%^&*()_+{}|:\"<>?\\-=[];',./]");

    if (password.contains(digitRegex))
        strength++;

    if (password.contains(lowerRegex))
        strength++;

    if (password.contains(upperRegex))
        strength++;

    if (password.contains(specialRegex))
        strength++;

    return strength;
}

void RegistrationWindow::attemptRegistration()
{
    QString login = loginLineEdit->text().trimmed();
    QString password = passwordLineEdit->text().trimmed();
    QString confirmPassword = confirmpasswordLineEdit->text().trimmed();

    if(password.length() < 8)
    {
        registrationErrorLabel->setText("Пароль должен быть\n не короче 8 символов.");
        return;
    }

    if ( login.isEmpty() || password.isEmpty() || confirmPassword.isEmpty())
    {
        registrationErrorLabel->setText("Заполните все поля.");
        return;
    }

    if (password != confirmPassword)
    {
        registrationErrorLabel->setText("Пароли не совпадают.");
        return;
    }

    int strength = calculatePasswordStrength(password);
    if(strength < minRequiredStrength)
    {
        registrationErrorLabel->setText("Пароль недостаточно надежен.\n"
                                        "Попробуйте сочетание букв,\n цифр и символов.");
        return;
    }

    Database db;

    if(db.openDatabase())
    {
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT login FROM Accounts WHERE login = ?");
        checkQuery.addBindValue(login);

        if(checkQuery.exec() && checkQuery.next()){
            registrationErrorLabel->setText("Такой логин уже существует");
        }
        else{
            if (db.openDatabase())
            {
                QSqlQuery query;
                query.prepare("INSERT INTO Accounts (login, password) VALUES (?, ?)");
                query.addBindValue(login);
                query.addBindValue(password);

                if (!query.exec())
                {
                    qDebug() << "Error inserting account:" << query.lastError().text();
                    registrationErrorLabel->setText("Ошибка при регистрации");
                }
                else
                {

                    qDebug() << "Account registered successfully!";
                    accept();
                }

                db.closeDatabase();
            }
            else
            {
                registrationErrorLabel->setText("Ошибка подключения к базе данных");
            }
        }
    }
}




