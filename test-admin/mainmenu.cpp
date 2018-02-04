#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    this->setWindowTitle("Test Constructor");

    ui->stackedWidget->setCurrentIndex(0);

    ui->addQuestionButton->setEnabled(false);
    ui->editQuestionButton->setEnabled(false);
    ui->deleteQuestionButton->setEnabled(false);
    ui->deleteTableButton->setEnabled(false);
    ui->settingsButton->setEnabled(false);

    ui->label->setAlignment(Qt::AlignCenter);
    ui->label_2->setAlignment(Qt::AlignCenter);
    ui->label_3->setAlignment(Qt::AlignCenter);
    ui->label_4->setAlignment(Qt::AlignCenter);
    ui->settingsButton->setStyleSheet("border-image: url(:/res/settingsInactive.png);"
                                      + QString("width: 50;")
                                      + "height: 50;");

    ui->listOfQuestions->setStyleSheet(

                "QListWidget::item:selected {"
                   "background-color:  rgba(80, 58, 46, 80);"
                "}");
    ui->listOfTables->setStyleSheet(
                "QListWidget::item:selected {"
                   "background-color:  rgba(80, 58, 46, 80);"
                "}");

    add_passwords();

    if(!QFile::exists(qApp->applicationDirPath() + "/tests/questions.db"))
    {
        QFile file(qApp->applicationDirPath() + "/tests/questions.db");
        file.open(QIODevice::WriteOnly);
        file.close();

        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(qApp->applicationDirPath() + "/tests/questions.db");
        database.open();

        QSqlQuery query(database);

        QString quer = "create table settings_of_tables"
                + QString(" (nameOfTable TEXT,")
                + " timeForTest INTEGER,"
                + " numOfQuestions INTEGER);";

        query.exec(quer);

        database.close();
    }

    connect(this, SIGNAL(emit_update_table()), this, SLOT(update_table_list()));
    connect(this, SIGNAL(emit_update_questions()), this, SLOT(update_question_list()));
    connect(this, SIGNAL(emit_delete_all_radios()), this, SLOT(delete_all_radios()));
    connect(this, SIGNAL(emit_clear_adding_question()), this, SLOT(clear_adding_question()));
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::add_passwords()
{
    passwords.push_back("6fv7p4eput");
    passwords.push_back("xrt3x87pff");
    passwords.push_back("jv38bnm6ux");
    passwords.push_back("pavlenko1998");
}

void MainMenu::on_loginButton_clicked()
{
    for(int i = 0; i < passwords.size(); i++)
    {
        if(ui->passwordBox->text() == passwords.at(i))
        {
            ui->stackedWidget->setCurrentIndex(1);
            update_table_list();
        }
    }
}

void MainMenu::update_question_list()
{
    ui->listOfQuestions->clear();

    ui->numOfQuestionsLabel->setText(QString::number(ui->listOfQuestions->count()));

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(qApp->applicationDirPath() + "/tests/questions.db");
    database.open();

    QSqlQuery query(database);

    QString que = "SELECT * FROM " + currentTable;
    query.exec(que);

    while(query.next())
    {
        ui->listOfQuestions->addItem(crack_cipher(query.value(0).toString()));
    }

    database.close();

    ui->numOfQuestionsLabel->setText(QString::number(ui->listOfQuestions->count()));
}

void MainMenu::update_table_list()
{
        ui->listOfTables->clear();
        ui->listOfQuestions->clear();

        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(qApp->applicationDirPath() + "/tests/questions.db");
        database.open();

        QSqlQuery query(database);

        QString que = "SELECT * FROM sqlite_master";
        query.exec(que);

        while(query.next())
        {
            QString name = query.value(1).toString();

            if(name == "settings_of_tables")
            {
                continue;
            }

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

            ui->listOfTables->addItem(finalName);
        }

        database.close();

        ui->numOfTablesLabel->setText(QString::number(ui->listOfTables->count()));
}

void MainMenu::on_addTableButton_clicked()
{
    this->setEnabled(false);
    addingTable *table = new addingTable(this);
    table->setAttribute(Qt::WA_DeleteOnClose);
    table->show();
}

void MainMenu::update()
{
    update_table_list();
    update_question_list();
}

void MainMenu::on_listOfTables_itemDoubleClicked(QListWidgetItem *item)
{
    ui->addQuestionButton->setEnabled(true);
    ui->editQuestionButton->setEnabled(true);
    ui->settingsButton->setEnabled(true);


    ui->settingsButton->setStyleSheet("border-image: url(:/res/settings.png);"
                                      + QString("width: 50;")
                                      + "height: 50;");

    currentTable = item->text();

    emit update_question_list();
}

void MainMenu::on_deleteTableButton_clicked()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(qApp->applicationDirPath() + "/tests/questions.db");
    database.open();

    QSqlQuery query(database);

    QString que = "DROP TABLE " + ui->listOfTables->currentItem()->text();
    query.exec(que);

    if(ui->listOfTables->currentItem()->text() == currentTable)
    {
        currentTable = "";
        emit update_question_list();

        ui->deleteQuestionButton->setEnabled(false);
        ui->addQuestionButton->setEnabled(false);
        ui->editQuestionButton->setEnabled(false);
        ui->settingsButton->setEnabled(false);
        ui->deleteTableButton->setEnabled(false);

        ui->settingsButton->setStyleSheet("border-image: url(:/res/settingsInactive.png);"
                                          + QString("width: 50;")
                                          + "height: 50;");
    }

    database.close();

    emit emit_update_table();
}

void MainMenu::delete_all_radios()
{
    ui->firstAnswerCorrect->setChecked(false);
    ui->secondAnswerCorrect->setChecked(false);
    ui->thirdAnswerCorrect->setChecked(false);
    ui->fourthAnswerCorrect->setChecked(false);
}

void MainMenu::on_firstAnswerCorrect_clicked()
{
    emit delete_all_radios();
    ui->firstAnswerCorrect->setChecked(true);
}

void MainMenu::on_secondAnswerCorrect_clicked()
{
    emit delete_all_radios();
    ui->secondAnswerCorrect->setChecked(true);
}

void MainMenu::on_thirdAnswerCorrect_clicked()
{
    emit delete_all_radios();
    ui->thirdAnswerCorrect->setChecked(true);
}

void MainMenu::on_fourthAnswerCorrect_clicked()
{
    emit delete_all_radios();
    ui->fourthAnswerCorrect->setChecked(true);
}

void MainMenu::on_resetButton_clicked()
{
    emit emit_clear_adding_question();
}

void MainMenu::clear_adding_question()
{
    emit delete_all_radios();

    ui->firstAnswer->clear();
    ui->secondAnswer->clear();
    ui->thirdAnswer->clear();
    ui->fourthAnswer->clear();

    ui->nameOfQuestionEdit->clear();
}

void MainMenu::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    emit emit_clear_adding_question();
}

QString MainMenu::create_cipher(QString input)
{
    QString output = "";
    QString in = "йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890 ";
    QString out = "юбьтимсчяэждлорпавыфъхзщшгнекуцйЮБЬТИМСЧЯЭЖДЛОРПАВЫФЪХЗЩШГНЕКУЦЙMNBVCXZLKJHGFDSAPOIUYTREWQmnbvcxzlkjhgfdsapoiuytrewq0987654321*";

    for(int i = 0; i < input.size(); i++)
    {
        for(int j = 0; j < in.size(); j++)
        {
            if(input.at(i) == in.at(j))
            {
                output += out.at(j);
            }
        }
    }

    return output;
}

QString MainMenu::crack_cipher(QString input)
{
    QString output = "";
    QString in = "йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890 ";
    QString out = "юбьтимсчяэждлорпавыфъхзщшгнекуцйЮБЬТИМСЧЯЭЖДЛОРПАВЫФЪХЗЩШГНЕКУЦЙMNBVCXZLKJHGFDSAPOIUYTREWQmnbvcxzlkjhgfdsapoiuytrewq0987654321*";

    for(int i = 0; i < input.size(); i++)
    {
        for(int j = 0; j < out.size(); j++)
        {
            if(input.at(i) == out.at(j))
            {
                output += in.at(j);
            }
        }
    }

    return output;
}

void MainMenu::on_saveQuestionButton_clicked()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(qApp->applicationDirPath() + "/tests/questions.db");
    database.open();

    QSqlQuery query(database);
    QString que;

    for(int i = 0; i < ui->listOfQuestions->count(); i++)
    {
        if(ui->nameOfQuestionEdit->text() == ui->listOfQuestions->takeItem(i)->text())
        {
            QMessageBox::critical(this, "Error", "It can't be same names of tests!");
            return;
        }
    }

    if(ui->nameOfQuestionEdit->text().isEmpty() || ui->firstAnswer->text().isEmpty()
            || ui->secondAnswer->text().isEmpty() || ui->thirdAnswer->text().isEmpty()
            || ui->fourthAnswer->text().isEmpty())
    {
        QMessageBox::critical(this, "Error", "Fill all fields!");
        return;
    }

    if((ui->firstAnswer->text() == ui->secondAnswer->text()
            || ui->firstAnswer->text() == ui->thirdAnswer->text()
            || ui->firstAnswer->text() == ui->fourthAnswer->text())
            || ui->secondAnswer->text() == ui->thirdAnswer->text()
            || ui->secondAnswer->text() == ui->fourthAnswer->text()
            || ui->thirdAnswer->text() == ui->fourthAnswer->text())
    {
        QMessageBox::critical(this, "Error", "It can't be same fields!");
        return;
    }

    if(isQuestionNew)
    {
        que = "INSERT INTO " + currentTable;
        que += QString(" (questionText,")
                + " firstAnswer,"
                + " secondAnswer,"
                + " thirdAnswer,"
                + " fourthAnswer,"
                + " correctAnswer) VALUES ('"
                + create_cipher(ui->nameOfQuestionEdit->text()) + "', '"
                + create_cipher(ui->firstAnswer->text()) + "', '"
                + create_cipher(ui->secondAnswer->text()) + "', '"
                + create_cipher(ui->thirdAnswer->text()) + "', '"
                + create_cipher(ui->fourthAnswer->text()) + "', '";
    }
    else
    {
        que = "UPDATE " + currentTable;
        que += " SET questionText = '"
                + create_cipher(ui->nameOfQuestionEdit->text()) + "', "
                + " firstAnswer = '"
                + create_cipher(ui->firstAnswer->text()) + "', "
                + " secondAnswer = '"
                + create_cipher(ui->secondAnswer->text()) + "', "
                + " thirdAnswer = '"
                + create_cipher(ui->thirdAnswer->text()) + "', "
                + " fourthAnswer = '"
                + create_cipher(ui->fourthAnswer->text()) + "',  correctAnswer = '";
    }

    if(ui->firstAnswerCorrect->isChecked())
    {
        que += create_cipher(ui->firstAnswer->text());
    }
    else if(ui->secondAnswerCorrect->isChecked())
    {
        que += create_cipher(ui->secondAnswer->text());
    }
    else if(ui->thirdAnswerCorrect->isChecked())
    {
        que += create_cipher(ui->thirdAnswer->text());
    }
    else if(ui->fourthAnswerCorrect->isChecked())
    {
        que += create_cipher(ui->fourthAnswer->text());
    }

    if(isQuestionNew)
    {
        que += "')";
    }
    else
    {
        que += "' WHERE questionText = '"
                + create_cipher(currentQuestion)
                + "'";
    }

    qDebug() << que;
    query.exec(que);

    database.close();

    emit emit_update_questions();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainMenu::on_editQuestionButton_clicked()
{
    emit emit_clear_adding_question();

    isQuestionNew = false;
    currentQuestion = ui->listOfQuestions->currentItem()->text();

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(qApp->applicationDirPath() + "/tests/questions.db");
    database.open();

    QSqlQuery query(database);

    QString que = "SELECT * FROM "
            + currentTable
            + " WHERE questionText = '"
            + create_cipher(currentQuestion)
            + "'";
    query.exec(que);

    QString correctAnsw;
    while(query.next())
    {
        ui->nameOfQuestionEdit->setText(crack_cipher(query.value("questionText").toString()));
        ui->firstAnswer->setText(crack_cipher(query.value("firstAnswer").toString()));
        ui->secondAnswer->setText(crack_cipher(query.value("secondAnswer").toString()));
        ui->thirdAnswer->setText(crack_cipher(query.value("thirdAnswer").toString()));
        ui->fourthAnswer->setText(crack_cipher(query.value("fourthAnswer").toString()));
        correctAnsw = crack_cipher(query.value("correctAnswer").toString());
    }

    if(ui->firstAnswer->text() == correctAnsw)
    {
        ui->firstAnswerCorrect->setChecked(true);
    }
    else if(ui->secondAnswer->text() == correctAnsw)
    {
        ui->secondAnswerCorrect->setChecked(true);
    }
    else if(ui->thirdAnswer->text() == correctAnsw)
    {
        ui->thirdAnswerCorrect->setChecked(true);
    }
    else if(ui->fourthAnswer->text() == correctAnsw)
    {
        ui->fourthAnswerCorrect->setChecked(true);
    }

    database.close();

    ui->stackedWidget->setCurrentIndex(2);
}

void MainMenu::on_addQuestionButton_clicked()
{
    isQuestionNew = true;
    emit emit_clear_adding_question();

    ui->firstAnswerCorrect->setChecked(true);
    ui->stackedWidget->setCurrentIndex(2);
}

void MainMenu::on_deleteQuestionButton_clicked()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(qApp->applicationDirPath() + "/tests/questions.db");
    database.open();

    QSqlQuery query(database);

    QString que = "DELETE FROM "
            + currentTable
            + " WHERE questionText = '"
            + create_cipher(ui->listOfQuestions->currentItem()->text())
            + "'";
    query.exec(que);

    database.close();

    emit emit_update_questions();

    ui->deleteQuestionButton->setEnabled(false);
}

void MainMenu::on_listOfQuestions_itemDoubleClicked(QListWidgetItem *item)
{
    on_editQuestionButton_clicked();
}

void MainMenu::on_settingsButton_clicked()
{
    this->setEnabled(false);

    settingsTable *Settings = new settingsTable(this, currentTable);
    Settings->setAttribute(Qt::WA_DeleteOnClose);
    Settings->show();
}

void MainMenu::save_settings_for_table(QString time, QString numberOfQuestionsForStudents)
{
    bool isOld = false;

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(qApp->applicationDirPath() + "/tests/questions.db");
    database.open();

    QSqlQuery query(database);

    QString que = "SELECT * FROM settings_of_tables WHERE nameOfTable = '"
            + currentTable + "'";
    query.exec(que);

    if(query.next())
    {
        isOld = true;
    }

    if(isOld)
    {
        QString que = "UPDATE settings_of_tables"
                + QString(" SET")
                + " timeForTest = "
                + time + ", "
                + " numOfQuestions = "
                + numberOfQuestionsForStudents
                + " WHERE nameOfTable = '"
                + currentTable + "'";

        query.exec(que);
    }

    database.close();
}

void MainMenu::on_listOfQuestions_itemClicked(QListWidgetItem *item)
{
    ui->deleteQuestionButton->setEnabled(true);
}

void MainMenu::on_listOfTables_itemClicked(QListWidgetItem *item)
{
    ui->deleteQuestionButton->setEnabled(true);
}
