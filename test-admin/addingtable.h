#ifndef ADDINGTABLE_H
#define ADDINGTABLE_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>

namespace Ui {
class addingTable;
}

class addingTable : public QDialog
{
    Q_OBJECT

public:
    explicit addingTable(QWidget *parent = 0);
    ~addingTable();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::addingTable *ui;

    QSqlDatabase database1 = QSqlDatabase::addDatabase("QSQLITE");

signals:
    void refresh_list();
};

#endif // ADDINGTABLE_H
