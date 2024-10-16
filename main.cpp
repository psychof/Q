#include "MainWindow.h"
#include "loginwindow.h"
#include "qdialog.h"


#include <QMainWindow>
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <database.h>


int main(int argc, char *argv[])
{


    QApplication app(argc, argv);
    LoginWindow loginWindow;

    Database db;

    if(db.openDatabase()){

    db.createAccountsTable();
    db.createTaskTable();

    }

    if(loginWindow.exec() == QDialog::Accepted) {            

        MainWindow w;
        w.show();
        return app.exec();

    } else {
     return 0;
    }
}
