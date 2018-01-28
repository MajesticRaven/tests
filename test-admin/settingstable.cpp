#include "settingstable.h"
#include "ui_settingstable.h"

settingsTable::settingsTable(QWidget *parent, QString currentTable) :
    QDialog(parent),
    ui(new Ui::settingsTable)
{
    ui->setupUi(this);

    this->setWindowTitle("Settings");
    this->setFixedSize(440, 170);

    ui->timeEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));
    ui->numberQuestionsEdit->setValidator(new QRegExpValidator(QRegExp("^([1-9][0-9]*|0)(\\.|,)[0-9]{2}"), this));

    connect(this, SIGNAL(send_the_settings(QString, QString)), parent, SLOT(save_settings_for_table(QString,QString)));

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(qApp->applicationDirPath() + "/tests/questions.db");
    database.open();

    QSqlQuery query(database);

    QString que = "SELECT * FROM settings_of_tables WHERE nameOfTable = '"
            + currentTable + "'";
    query.exec(que);

    if(query.next())
    {
        ui->timeEdit->setText(query.value(1).toString());
        ui->numberQuestionsEdit->setText(query.value(2).toString());
    }

    database.close();
}

settingsTable::~settingsTable()
{
    delete ui;
}

void settingsTable::on_cancelButton_clicked()
{
    parentWidget()->setEnabled(true);
    this->close();
}

void settingsTable::on_okButton_clicked()
{
    if(ui->numberQuestionsEdit->text().toInt() > 20
            || ui->numberQuestionsEdit->text().toInt() < 5)
    {
        QMessageBox::critical(this, "Error", "Enter number of test more than 5 and less than 20");
        return;
    }

    if(ui->timeEdit->text().toInt() > 25
            || ui->timeEdit->text() < 5)
    {
        QMessageBox::critical(this, "Error", "Enter time for testing more than 5 and less than 25");
        return;
    }

    emit send_the_settings(QString(ui->timeEdit->text()), QString(ui->numberQuestionsEdit->text()));

    parentWidget()->setEnabled(true);
    this->close();
}
