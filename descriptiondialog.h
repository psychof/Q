#ifndef DESCRIPTIONDIALOG_H
#define DESCRIPTIONDIALOG_H

#include "qtextedit.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QKeyEvent>
#include <QLabel>

class QTextEdit;

class DescriptionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DescriptionDialog(const QString &taskName, QWidget *parent = nullptr);
    QString getDescription() const;

private:
    QTextEdit *descriptionTextEdit;
};

#endif // DESCRIPTIONDIALOG_H
