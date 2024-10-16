#include "descriptiondialog.h"


DescriptionDialog::DescriptionDialog(const QString &taskName, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Описание: " + taskName);

    descriptionTextEdit = new QTextEdit(this);

    descriptionTextEdit->resize(350,350);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(descriptionTextEdit);

}















