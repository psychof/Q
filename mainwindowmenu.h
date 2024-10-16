#ifndef MAINWINDOWMENU_H
#define MAINWINDOWMENU_H


#include <QObject>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>
#include <QPushButton>
#include <QShortcut>



class MenuBar : public QObject
{
    Q_OBJECT

public:
    explicit MenuBar(QMainWindow *parent = nullptr);

private:
    QMainWindow *m_parent;
    QWidget *searchWidget;
    QLineEdit *searchLineEdit;


    void sortTasksByDate(bool ascending);
    void sortTasksByPriority(bool ascending);
    void createMenu();

public slots:
    void toggleSearchWidget();
    void searchTasks(const QString& searchText);



private slots:
    void aboutProgramWindow();



};

#endif // MAINWINDOWMENU_H


