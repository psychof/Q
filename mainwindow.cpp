#include "mainwindow.h"
#include "loginwindow.h"
#include "ui_mainwindow.h"
#include "mainwindowmenu.h"
#include "database.h"
#include "sessionmanager.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), verticalLayout(new QVBoxLayout) {
    ui->setupUi(this);

    setWindowTitle("WindowsOrgonizer");






    QScrollArea* scrollArea = new QScrollArea(this); //устонавливает scrollarea // this передаётся как родительский виджет для области прокрутки. В данном контексте this — это главный виджет приложения (MainWindow), который наследует QMainWindow. Таким образом, область прокрутки прикрепляется к главному окну.
    QWidget* containerWidget = new QWidget;
    containerWidget->setLayout(verticalLayout);

    scrollArea->setWidgetResizable(true); //Эта строка включает возможность изменения размера содержимого в зависимости от размера области прокрутки.
    scrollArea->setWidget(containerWidget);
    setCentralWidget(scrollArea); //устоновка scrollArea в качестве центрального виджета



    new MenuBar(this); //Создаётся объект MenuBar и привязывается к главному окну (this).

    loadTasksFromDatabase(); //загрузка задач из базы данных при звпуске приложении


}

QWidget* MainWindow::createTaskWidget(int Id) {

    QWidget* taskWidget = new QWidget(this);

    // Основной вертикальный макет для виджета задачи
    QVBoxLayout* mainLayout = new QVBoxLayout(taskWidget);





    // Горизонтальный макет для основной строки ввода и других элементов
    QHBoxLayout* layout = new QHBoxLayout();



    QLineEdit* lineEdit = new QLineEdit; //поле для ввода названия задачи
    QTextEdit* descriptiontextEdit = new QTextEdit; //поле для описание задачи
    QComboBox* comboBox = new QComboBox; //поле для приоритета
    QDateEdit* dateEdit = new QDateEdit(QDate::currentDate(), taskWidget); //поле даты
    QTimeEdit* timeEdit = new QTimeEdit(QTime::currentTime(), taskWidget); //поле время
    dateEdit->setCalendarPopup(true); // устоновка свойства чтобы dateEdit показывал сплывающий календарь при выборе даты

    comboBox->addItem("Низкий", QVariant::fromValue(int(TaskPriority::Low)));
    comboBox->addItem("Средний", QVariant::fromValue(int(TaskPriority::Medium)));  //добовление трёх элементов в выпадающий список combotBox каждый пункт связан с соответствующим значением в перечесление TaskPriotity
    comboBox->addItem("Высокий", QVariant::fromValue(int(TaskPriority::High)));

    //добовленеи виджетов (даты,времени,поля для вода,приорететы) в HBoxLayout который размещает их в одну строчку
    layout->addWidget(lineEdit);
    layout->addWidget(comboBox);
    layout->addWidget(dateEdit);
    layout->addWidget(timeEdit);

    // Горизонтальный компоновщик layout добавляется в основной вертикальный компоновщик mainLayout
    mainLayout->addLayout(layout);


    // Описание задачи (изначально скрытое)
    descriptiontextEdit->setPlaceholderText("Описание задачи");
    mainLayout->addWidget(descriptiontextEdit);//добовление текстового поля в вертикальный компоновщик
    descriptiontextEdit->setVisible(false); //устанавливает видимость описание по умолчанию скрытой

    // Настройка пропорций размеров
    layout->setStretch(0, 8);
    layout->setStretch(1, 1);
    layout->setStretch(2, 1);
    layout->setStretch(3, 1);



    // Добавляем виджет задачи в основной макет окна
    verticalLayout->addWidget(taskWidget);
    // Настройка контекстного меню для строки ввода
    setupContextMenu(lineEdit, taskWidget);
    lineEdit->setObjectName("taskLineEdit"); //присваивает полю ввода имя "taskLineEdit"
    taskWidget->setProperty("taskId", Id);  //Устанавливается свойство taskId для виджета задачи, чтобы хранить идентификатор задачи.

    return taskWidget; //Возвращается указатель на созданный виджет задачи.
}


QWidget* MainWindow::createTaskWidgetFromDb(int taskId, const QString& taskName, int priority, const QDate& dueDate, const QTime& dueTime, const QString& description) {

    QWidget* taskWidget = new QWidget(this);

    // Основной вертикальный макет для виджета задачи
    QVBoxLayout* mainLayout = new QVBoxLayout(taskWidget);

    // Горизонтальный макет для основной строки ввода и других элементов
    QHBoxLayout* layout = new QHBoxLayout();
    QLineEdit* lineEdit = new QLineEdit(taskName);
    QTextEdit* descriptiontextEdit = new QTextEdit(description);
    QComboBox* comboBox = new QComboBox;
    QDateEdit* dateEdit = new QDateEdit(dueDate, taskWidget);
    QTimeEdit* timeEdit = new QTimeEdit(dueTime, taskWidget);
    dateEdit->setCalendarPopup(true);

    comboBox->addItem("Низкий", QVariant::fromValue(int(TaskPriority::Low)));
    comboBox->addItem("Средний", QVariant::fromValue(int(TaskPriority::Medium)));
    comboBox->addItem("Высокий", QVariant::fromValue(int(TaskPriority::High)));

    layout->addWidget(lineEdit);
    layout->addWidget(comboBox);
    layout->addWidget(dateEdit);
    layout->addWidget(timeEdit);

    // Добавляем горизонтальный макет в основной вертикальный макет
    mainLayout->addLayout(layout);

    // Описание задачи (изначально скрытое)
    descriptiontextEdit->setPlaceholderText("Описание задачи");
    mainLayout->addWidget(descriptiontextEdit);

    descriptiontextEdit->setVisible(false);

    // Настройка пропорций размеров
    layout->setStretch(0, 8);
    layout->setStretch(1, 1);
    layout->setStretch(2, 1);
    layout->setStretch(3, 1);

    // Добавляем виджет задачи в основной макет окна
    verticalLayout->addWidget(taskWidget);



    setupContextMenu(lineEdit, taskWidget);

    lineEdit->setObjectName("taskLineEdit");
    comboBox->setCurrentIndex(comboBox->findData(priority));
    taskWidget->setProperty("taskId", taskId);
    descriptiontextEdit->setVisible(false);


    return taskWidget;
}


void MainWindow::setupContextMenu(QLineEdit* lineEdit, QWidget* taskWidget) {


    QMenu* contextMenu = new QMenu(lineEdit);
    QAction* deleteAction = contextMenu->addAction("Удалить задачу");
    QAction* descriptionAction = contextMenu->addAction("Описание");


    connect(deleteAction, &QAction::triggered, this, [this, taskWidget]() {

        QMessageBox::StandardButton _reply;
        _reply = QMessageBox::question(this, "Удаление", "Вы уверены, что хотите удалить эту задачу?",
                                QMessageBox::Yes | QMessageBox::No);

        if(_reply == QMessageBox::Yes){
         int taskId = taskWidget->property("taskId").toInt();
         removeTaskFromDatabase(taskId);
         verticalLayout->removeWidget(taskWidget);
         taskWidget->deleteLater(); //Удаление через deleteLater гарантирует, что объект не будет удален, пока не завершатся все операции, в которых он участвует.

        }
    });

    connect(descriptionAction, &QAction::triggered, this, [ taskWidget]() {
        QTextEdit* descriptionTextEdit = taskWidget->findChild<QTextEdit*>();
        if (descriptionTextEdit) {
            bool isVisible = descriptionTextEdit->isVisible();
            descriptionTextEdit->setVisible(!isVisible); // Переключение видимости
        }
    });


    lineEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(lineEdit, &QLineEdit::customContextMenuRequested, [lineEdit, contextMenu](const QPoint& pos) {
        contextMenu->exec(lineEdit->mapToGlobal(pos));
    });
}

void MainWindow::GetTasks()
{

    for (int i = 0; i < verticalLayout->count(); ++i) {
        QWidget* widget = verticalLayout->itemAt(i)->widget();
        if (widget) {
            QLineEdit* lineEdit = widget->findChild<QLineEdit*>();
            QComboBox* comboBox = widget->findChild<QComboBox*>();
            QDateEdit* dateEdit = widget->findChild<QDateEdit*>();
            QTimeEdit* timeEdit = widget->findChild<QTimeEdit*>();
            QTextEdit* descriptiontextEdit = widget ->findChild<QTextEdit*>();

            int taskId = widget->property("taskId").toInt();

            if (lineEdit && comboBox && dateEdit && timeEdit) {
                QString taskName = lineEdit->text();
                int priority = comboBox->currentData().toInt();
                QDate dueDate = dateEdit->date();
                QTime dueTime = timeEdit->time();
                QString description = descriptiontextEdit->toPlainText();

                // Проверка на пустые или некорректные значения
                if (!taskName.isEmpty() && dueDate.isValid() && dueTime.isValid()) {
                    saveTaskToDatabase(taskName, priority, dueDate, dueTime,taskId,description);
                }
            }
        }
    }
}


void MainWindow::saveTaskToDatabase(const QString& taskName, int priority, const QDate& dueDate, const QTime& dueTime, int taskId,const QString &description)
{
    int userId = SessionManager::instance().getCurrentUserId();

    Database db;
    if (db.openDatabase()) {
        QSqlQuery query(db.getDatabase());

        if (taskId == 0) {
            // Вставка новой задачи
            query.prepare("INSERT INTO Tasks (taskName, priority, dueDate, dueTime, userId, description) VALUES (:taskName, :priority, :dueDate, :dueTime, :userId, :description) RETURNING id");
            query.bindValue(":taskName", taskName);
            query.bindValue(":priority", priority);
            query.bindValue(":dueDate", dueDate.toString(Qt::ISODate));
            query.bindValue(":dueTime", dueTime.toString(Qt::ISODate));
            query.bindValue(":description", description);
            query.bindValue(":userId", userId);

            if (query.exec()) {
                if (query.next()) {
                    int newTaskId = query.value(0).toInt();
                    qDebug() << "Task inserted with ID:" << newTaskId;
                } else {
                    qDebug() << "Error retrieving new task ID.";
                }
            } else {
                qDebug() << "Error inserting task into database:" << query.lastError().text();
            }
        } else {
            // Обновление существующей задачи
            query.prepare("UPDATE Tasks SET taskName = :taskName, priority = :priority, dueDate = :dueDate, dueTime = :dueTime, description = :description WHERE id = :taskId AND userId = :userId");
            query.bindValue(":taskName", taskName);
            query.bindValue(":priority", priority);
            query.bindValue(":dueDate", dueDate.toString(Qt::ISODate));
            query.bindValue(":dueTime", dueTime.toString(Qt::ISODate));
            query.bindValue(":taskId", taskId);
            query.bindValue(":userId", userId);
            query.bindValue(":description", description);

            if (!query.exec()) {
                qDebug() << "Error updating task in database:" << query.lastError().text();
            } else {
                qDebug() << "Task updated successfully with ID:" << taskId;
            }
        }
    }
}

void MainWindow::removeTaskFromDatabase(int taskId){

    Database db;
    if(db.openDatabase()){
        QSqlQuery query(db.getDatabase());
        query.prepare("DELETE FROM Tasks WHERE id = :taskId");
        query.bindValue(":taskId",taskId);

        if(!query.exec()){
            qDebug() << "Error removing task from database:" << query.lastError().text();
        }

    }
}


void MainWindow::loadTasksFromDatabase()
{
    int userId = SessionManager::instance().getCurrentUserId();


    Database db;
    if (db.openDatabase()) {
        QSqlQuery query(db.getDatabase());
        query.prepare("SELECT id, taskName, priority, dueDate, dueTime, description FROM Tasks WHERE userId = :userId");
        query.bindValue(":userId", userId);

        if (query.exec()) {
            while (query.next()) {
                int taskId = query.value(0).toInt();
                QString taskName = query.value(1).toString();
                int priority = query.value(2).toInt();
                QDate dueDate = QDate::fromString(query.value(3).toString(), Qt::ISODate);
                QTime dueTime = QTime::fromString(query.value(4).toString(), Qt::ISODate);
                QString description = query.value(5).toString();

                createTaskWidgetFromDb(taskId, taskName, priority, dueDate, dueTime,description);
            }
        } else {
            qDebug() << "Error loading tasks from database:" << query.lastError().text();
        }
    }
}
void MainWindow::logoutCurrentUser() {

    // Сохранить все текущие задачи перед выходом из аккаунта
    GetTasks();

    // Показать диалоговое окно подтверждения выхода
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Выход", "Вы уверены, что хотите выйти из аккаунта?",
                                  QMessageBox::Yes | QMessageBox::No);

    // Проверить, нажал ли пользователь "Yes"
    if (reply == QMessageBox::Yes) {
        // Завершаем текущую сессию
        SessionManager::instance().clearSession();

        // Очищаем все задачи с экрана
        while (QLayoutItem* item = verticalLayout->takeAt(0)) {
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }

        // Закрываем текущее окно с задачами
        this->close();

        // Создаём окно входа
        LoginWindow* loginWindow = new LoginWindow;

        // Если вход выполнен успешно
        if (loginWindow->exec() == QDialog::Accepted) {
            // Создаём новое главное окно
            MainWindow* newMainWindow = new MainWindow;
            newMainWindow->show();
        }

        delete loginWindow;  // Освобождаем память
    }
}
void MainWindow::onUserLoggedIn() {
   logoutCurrentUser();
}
void MainWindow::searchTasks(const QString& searchText) {
    for (int i = 0; i < verticalLayout->count(); ++i) {
        QWidget* widget = verticalLayout->itemAt(i)->widget();
        if (widget) {
            QLineEdit* lineEdit = widget->findChild<QLineEdit*>("taskLineEdit");
            if (lineEdit) {
                bool matches = lineEdit->text().contains(searchText, Qt::CaseInsensitive);
                widget->setVisible(matches);
            }
        }
    }
}

void MainWindow::sortTasksByDate(bool ascending) {
    QVector<QWidget*> widgets;
    for (int i = 0; i < verticalLayout->count(); ++i) {
        QWidget* widget = verticalLayout->itemAt(i)->widget();
        if (widget) {
            widgets.append(widget);
        }
    }

    std::sort(widgets.begin(), widgets.end(), [ascending](QWidget* a, QWidget* b) {
        QDateEdit* dateEditA = a->findChild<QDateEdit*>();
        QTimeEdit* timeEditA = a->findChild<QTimeEdit*>();
        QDateEdit* dateEditB = b->findChild<QDateEdit*>();
        QTimeEdit* timeEditB = b->findChild<QTimeEdit*>();

        if (dateEditA && timeEditA && dateEditB && timeEditB) {
            QDateTime dateTimeA(dateEditA->date(), timeEditA->time());
            QDateTime dateTimeB(dateEditB->date(), timeEditB->time());

            if (ascending) {
                return dateTimeA < dateTimeB;
            } else {
                return dateTimeA > dateTimeB;
            }
        }
        return false;
    });

    while (QLayoutItem* item = verticalLayout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->setParent(nullptr);
        }
        delete item;
    }

    for (QWidget* widget : widgets) {
        verticalLayout->addWidget(widget);
    }
}

void MainWindow::sortTasksByPriority(bool ascending) {
    QVector<QWidget*> widgets;
    for (int i = 0; i < verticalLayout->count(); ++i) {
        QWidget* widget = verticalLayout->itemAt(i)->widget();
        if (widget) {
            widgets.append(widget);
        }
    }

    std::sort(widgets.begin(), widgets.end(), [ascending](QWidget* a, QWidget* b) {
        QComboBox* comboBoxA = a->findChild<QComboBox*>();
        QComboBox* comboBoxB = b->findChild<QComboBox*>();

        if (comboBoxA && comboBoxB) {
            int priorityA = comboBoxA->currentData().toInt();
            int priorityB = comboBoxB->currentData().toInt();

            if (ascending) {
                return priorityA < priorityB;
            } else {
                return priorityA > priorityB;
            }
        }
        return false;
    });

    while (QLayoutItem* item = verticalLayout->takeAt(0)) {
        if (item->widget()) {
            item->widget()->setParent(nullptr);
        }
        delete item;
    }

    for (QWidget* widget : widgets) {
        verticalLayout->addWidget(widget);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {

    GetTasks();


    QMainWindow::closeEvent(event);
}


MainWindow::~MainWindow() {

    delete ui;
}
