#include "mainwindowmenu.h"
#include "MainWindow.h"
#include "qboxlayout.h"
#include "qdialog.h"
#include "qmessagebox.h"
#include "qtextedit.h"

MenuBar::MenuBar(QMainWindow *parent):QObject(parent),m_parent(parent){
    createMenu();
}

void MenuBar::createMenu(){

    if (!m_parent) {
        qDebug() << "Error: Invalid parent QMainWindow pointer.";
        return;
    }

    MainWindow *mainWindow = qobject_cast<MainWindow*>(parent());
    QMenuBar *menuBar = m_parent->menuBar();
    QMenu* menu = menuBar->addMenu("Дополнительно");

    QAction *addAction = new QAction("Добавить задачу",m_parent);
    if(mainWindow){
        connect(addAction, &QAction::triggered, mainWindow, &MainWindow::createTaskWidget);
    }

    QAction *searchAction = new QAction("Поиск", m_parent);
    connect(searchAction, &QAction::triggered, this, &MenuBar::toggleSearchWidget);

    // Добавление меню фильтрации
    QMenu* filterMenu = new QMenu("Фильтрация", m_parent);

    // Подменю для фильтрации по дате
    QMenu* dateFilterMenu = new QMenu("По дате и времени", m_parent);
    QAction* dateAscAction = new QAction("По возрастанию", m_parent);
    QAction* dateDescAction = new QAction("По убыванию", m_parent);
    connect(dateAscAction, &QAction::triggered, this, [this]() { sortTasksByDate(true); });
    connect(dateDescAction, &QAction::triggered, this, [this]() { sortTasksByDate(false); });
    dateFilterMenu->addAction(dateAscAction);
    dateFilterMenu->addAction(dateDescAction);

    // Подменю для фильтрации по приоритету
    QMenu* priorityFilterMenu = new QMenu("По приоритету", m_parent);
    QAction* priorityAscAction = new QAction("По возрастанию", m_parent);
    QAction* priorityDescAction = new QAction("По убыванию", m_parent);
    connect(priorityAscAction, &QAction::triggered, this, [this]() { sortTasksByPriority(true); });
    connect(priorityDescAction, &QAction::triggered, this, [this]() { sortTasksByPriority(false); });
    priorityFilterMenu->addAction(priorityAscAction);
    priorityFilterMenu->addAction(priorityDescAction);

    filterMenu->addMenu(dateFilterMenu);
    filterMenu->addMenu(priorityFilterMenu);

    QAction *aboutAction = new QAction("О программе",m_parent);
    connect(aboutAction, &QAction::triggered,this,&MenuBar::aboutProgramWindow);

    QAction *logoutAction = new QAction("Выход из аккаунта",m_parent);
    if (mainWindow) {
        connect(logoutAction, &QAction::triggered, mainWindow, &MainWindow::logoutCurrentUser);
    } else {
        qDebug() << "Error: Could not cast parent to MainWindow.";
    }

    menu->addAction(addAction);
    menu->addAction(searchAction);
    menu->addMenu(filterMenu);
    menu->addAction(aboutAction);
    menu->addAction(logoutAction);

    searchWidget = new QWidget;
    searchWidget->setVisible(false);

    searchLineEdit = new QLineEdit;
    searchLineEdit->setPlaceholderText("Поиск...");

    QVBoxLayout *searchLayout = new QVBoxLayout(searchWidget);
    searchLayout->addWidget(searchLineEdit);
    searchLayout->setContentsMargins(0, 0, 0, 0);

    menuBar->setCornerWidget(searchWidget, Qt::TopRightCorner);

    connect(searchLineEdit, &QLineEdit::textChanged, this, &MenuBar::searchTasks);
}

void MenuBar::toggleSearchWidget() {
    bool isVisible = searchWidget->isVisible();
    searchWidget->setVisible(!isVisible);

    if (!isVisible) {
        searchLineEdit->setFocus();
    }
}

void MenuBar::searchTasks(const QString& searchText) {
    if (m_parent) {
        MainWindow* mainWindow = qobject_cast<MainWindow*>(m_parent);
        if (mainWindow) {
            mainWindow->searchTasks(searchText);
        }
    }
}

void MenuBar::sortTasksByDate(bool ascending) {
    if (m_parent) {
        MainWindow* mainWindow = qobject_cast<MainWindow*>(m_parent);
        if (mainWindow) {
            mainWindow->sortTasksByDate(ascending);
        }
    }
}

void MenuBar::sortTasksByPriority(bool ascending) {
    if (m_parent) {
        MainWindow* mainWindow = qobject_cast<MainWindow*>(m_parent);
        if (mainWindow) {
            mainWindow->sortTasksByPriority(ascending);
        }
    }
}

void MenuBar::aboutProgramWindow() {
    QDialog *aboutDialog = new QDialog;

    aboutDialog->setWindowTitle("О программе");
    aboutDialog->resize(100,100);

    QVBoxLayout *layout = new QVBoxLayout(aboutDialog);

    QLabel *versionLabel = new QLabel("Версия 0.0.1");
    QLabel *lastUpdateLabel = new QLabel("Последнее обновление: ......");
    QLabel *descriptionText = new QLabel("Описание программы:");

    versionLabel->setParent(aboutDialog);
    lastUpdateLabel->setParent(aboutDialog);
    descriptionText->setParent(aboutDialog);

    layout->setContentsMargins(20, 10, 10, 10);
    layout->setSpacing(15);

    layout->addWidget(versionLabel);
    layout->addWidget(lastUpdateLabel);
    layout->addWidget(descriptionText);

    aboutDialog->exec();
}


