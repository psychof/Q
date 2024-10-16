#include "database.h"

Database::Database()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QPSQL");
    }
}


bool Database::openDatabase() {
    if (!db.isOpen()) {
        db.setHostName("localhost");
        db.setPort(5432);
        db.setDatabaseName("postgres");
        db.setUserName("postgres");
        db.setPassword("123456QWERTASDFZXC");
        if (!db.open()) {
            qDebug() << "Error opening connection:"  << db.lastError().text();
            return false;
        }
        qDebug() << "Connection to the database opened successfully.";
    }
    return true;
}


QSqlDatabase& Database::getDatabase()
{
    return db;
}


void Database::createAccountsTable()
{
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS Accounts ("
               "id SERIAL PRIMARY KEY,"
               "login TEXT NOT NULL,"
               "password TEXT NOT NULL"
               ")");

    if (query.isActive()) {
        qDebug() << "Table 'Accounts' created successfully or already exists.";
    } else {
        qDebug() << "Error creating table 'Accounts':" << query.lastError().text();
    }
}

void Database::createTaskTable()
{
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS Tasks ("
               "id INTEGER PRIMARY KEY,"
               "taskName TEXT NOT NULL,"
               "priority INTEGER,"
               "dueDate DATE,"
               "dueTime TIME"
               ")");

    if (query.isActive()) {
        qDebug() << "Table 'Tasks' created or updated successfully.";
    } else {
        qDebug() << "Error creating or updating table 'Tasks':" << query.lastError().text();
    }
}

void Database::updateTables(){



}

void Database::closeDatabase() {

    if (db.isOpen()) {
        db.close();
        qDebug() << "Connection to the database closed.";
    }

}

