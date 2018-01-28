#include "addingtable.h"
#include "ui_addingtable.h"

addingTable::addingTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addingTable)
{
    ui->setupUi(this);
    this->setFixedSize(300, 128);
    this->setWindowTitle("New table");

    connect(this, SIGNAL(refresh_list()), parentWidget(), SLOT(update()));
}

addingTable::~addingTable()
{
    delete ui;
}

void addingTable::on_okButton_clicked()
{
    if(!ui->nameOfTable->text().isEmpty())
    {
        QString name_of_table = "";

        for(int i = 0; i < ui->nameOfTable->text().size(); i++)
        {
            if(ui->nameOfTable->text().at(i) == " ")
            {
                name_of_table += "_";
            }
            else
            {
                name_of_table += ui->nameOfTable->text().at(i);
            }
        }

        database1.setDatabaseName(qApp->applicationDirPath() + "/tests/questions.db");
        database1.open();
        QSqlQuery query(database1);

        QString quer = "create table ";
        quer += name_of_table
                + " (questionText TEXT,"
                + " firstAnswer TEXT,"
                + " secondAnswer TEXT,"
                + " thirdAnswer TEXT,"
                + " fourthAnswer TEXT,"
                + " correctAnswer TEXT);";
        query.exec(quer);

        QString conname = database1.connectionName();
        database1.close();
        database1.removeDatabase(conname);

        parentWidget()->setEnabled(true);

        emit refresh_list();
        this->close();
    }
}

void addingTable::on_cancelButton_clicked()
{
    parentWidget()->setEnabled(true);
    emit refresh_list();
    this->close();
}

