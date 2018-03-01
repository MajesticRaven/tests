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
    ui->nameOfTable->setValidator(new QRegExpValidator(QRegExp("^[A-zА-я ]+$"), this));
}

addingTable::~addingTable()
{
    parentWidget()->setEnabled(true);
    delete ui;
}

void addingTable::on_okButton_clicked()
{
    database1.setDatabaseName(qApp->applicationDirPath() + "/tests/questions.db");
    database1.open();

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(qApp->applicationDirPath() + "/tests/questions.db");
    database.open();

    QSqlQuery query(database);

    QString que = "SELECT * FROM sqlite_master";
    query.exec(que);

    while(query.next())
    {
        QString name = query.value(1).toString();
        QString finalName = "";

        for(int i = 0; i < name.size(); i++)
        {
            if(name.at(i) == "_")
            {
                finalName += " ";
            }
            else
            {
                finalName += name.at(i);
            }
        }

        if(ui->nameOfTable->text() == finalName)
        {
            QMessageBox::critical(this, "Error", "Enter original name of table!");
            return;
        }
    }

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

        quer = "INSERT INTO settings_of_tables"
                + QString(" (nameOfTable,")
                + " timeForTest,"
                + " numOfQuestions)"
                + " VALUES ('"
                + ui->nameOfTable->text()
                + "', 10, 12);";

        query.exec(quer);

        database.close();

        parentWidget()->setEnabled(true);

        emit refresh_list();
        this->close();
    }

    QString conname = database1.connectionName();
    database1.close();
    database1.removeDatabase(conname);
}

void addingTable::on_cancelButton_clicked()
{
    parentWidget()->setEnabled(true);
    emit refresh_list();
    this->close();
}

