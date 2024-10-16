#include "loginwindow.h"
#include "CustomLineEdit.h"
#include "qsqlquery.h"
#include "registrationwindow.h"
#include "database.h"


LoginWindow::LoginWindow(QWidget *parent) : QDialog(parent)
{

    setWindowTitle("Вход в аккаунт");

    QVBoxLayout *layout = new QVBoxLayout(this);

    loginLineEdit = new QLineEdit(this);
    loginLineEdit->setPlaceholderText("Логин");
    layout->addWidget(loginLineEdit);

    passwordLineEdit = new CustomLineEdit(this);
    passwordLineEdit->setPlaceholderText("Пароль");
    layout->addWidget(passwordLineEdit);

    rememberMeCheckBox = new QCheckBox("Запомните аккаунт",this);
    layout->addWidget(rememberMeCheckBox);


    loginErrorLabel = new QLabel(this);
    loginErrorLabel->setStyleSheet("color: red;");
    layout->addWidget(loginErrorLabel);


    loginButton = new QPushButton("Войти", this);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::attemptLogin);
    layout->addWidget(loginButton);

    registrButton = new QPushButton("Регестрация",this);
    connect(registrButton,&QPushButton::clicked,this, &LoginWindow::openRegistrationWindow);
    layout->addWidget(registrButton);

    QSettings settings("MyApp", "LoginSettings");
    bool rememberChecked = settings.value("rememberChecked", false).toBool();
    rememberMeCheckBox->setChecked(rememberChecked);

    if (rememberChecked) {
        QString savedLogin = settings.value("login", "").toString();
        QString savedPassword = settings.value("password", "").toString();
        loginLineEdit->setText(savedLogin);
        passwordLineEdit->setText(savedPassword);
    }
}

void LoginWindow::attemptLogin()
{
    QString login = loginLineEdit->text();
    QString password = passwordLineEdit->text();

    Database database;
    if (database.openDatabase()) {
        QSqlDatabase& db = database.getDatabase();

        QSqlQuery query(db);
        query.prepare("SELECT id FROM Accounts WHERE login = :login AND password = :password");
        query.bindValue(":login", login);
        query.bindValue(":password", password);

        if (query.exec() && query.next()) {
            QSettings settings("MyApp", "LoginSettings");
            settings.setValue("rememberChecked", rememberMeCheckBox->isChecked());

            if(rememberMeCheckBox->isChecked()){
                settings.setValue("login", login);
                settings.setValue("password", password);
            }else {
                settings.remove("login");
                settings.remove("password");
            }

           int userId = query.value(0).toInt();
           SessionManager::instance().setCurrentUserId(userId);


            accept();
        } else {
            loginErrorLabel->setText("Неправильный логин или пароль");
        }

        database.closeDatabase();
    } else {
        loginErrorLabel->setText("Ошибка подключения к базе данных");
    }
}

void LoginWindow::openRegistrationWindow()
{
    RegistrationWindow *registrationWindow = new RegistrationWindow(this);
    registrationWindow->exec();
}

