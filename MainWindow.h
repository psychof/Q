#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMenu>
#include <QAction>
#include <QVector>
#include <QScrollArea>
#include <QTime>
#include <QDate>
#include <QString>
#include <QCloseEvent>
#include <QTextEdit>



namespace Ui {
class MainWindow;
}

enum class TaskPriority {
    Low = 1,
    Medium = 2,
    High = 3
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    int userId;

    ~MainWindow();

public slots:
    QWidget *createTaskWidget(int Id);
    QWidget *createTaskWidgetFromDb(int taskId, const QString& taskName, int priority, const QDate& dueDate, const QTime& dueTime, const QString &description );
    void logoutCurrentUser();
    void onUserLoggedIn();
    void searchTasks(const QString &searchText);
    void sortTasksByDate(bool ascending);
    void sortTasksByPriority(bool ascending);





private:
    Ui::MainWindow *ui;
    QVBoxLayout *verticalLayout;
    void setupContextMenu(QLineEdit *lineEdit, QWidget *taskWidget);
    void saveTaskToDatabase(const QString& taskName, int priority, const QDate& dueDate, const QTime& dueTime, int taskId, const QString &description);
    void GetTasks();
    void loadTasksFromDatabase();
    void removeTaskFromDatabase(int taskId);

protected:
    void closeEvent(QCloseEvent *event) override;


};

#endif // MAINWINDOW_H
