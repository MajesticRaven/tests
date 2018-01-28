#ifndef SETTINGSTABLE_H
#define SETTINGSTABLE_H

#include <QDialog>
#include <QMessageBox>

#include "mainmenu.h"

namespace Ui {
class settingsTable;
}

class settingsTable : public QDialog
{
    Q_OBJECT

public:
    explicit settingsTable(QWidget *parent = 0);
    ~settingsTable();

private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

private:
    Ui::settingsTable *ui;

signals:
    send_the_settings(QString time, QString numberOfQuestionsForStudents);
};

#endif // SETTINGSTABLE_H
