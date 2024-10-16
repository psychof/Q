#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H



#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDate>
#include <QTime>



class Database
{

public:
    explicit Database();
    bool openDatabase();
    void closeDatabase();
    void createTaskTable();
    void createAccountsTable();
    QSqlDatabase& getDatabase();
    void updateTables();





private:
    QSqlDatabase db;






};


#endif // DATABASECONNECTION_H
