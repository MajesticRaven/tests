#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    this->setWindowTitle("Tests");
    srand(time(NULL));

    ui->stackedWidget->setCurrentIndex(0);
    ui->numberQuestionLabel->setAlignment(Qt::AlignCenter);
    ui->questionLabel->setAlignment(Qt::AlignCenter);
    ui->starsLabel->setAlignment(Qt::AlignCenter);
    ui->timeLeft->setAlignment(Qt::AlignCenter);
    ui->label_3->setAlignment(Qt::AlignCenter);

    QString qssText = "border: none; "
            + QString("border-image: url(:/images/rightArrow.png);")
            + "width: 70;"
            + "height: 70;";

    ui->nextQuestionButton->setStyleSheet(qssText);

    qssText = "border: none; "
            + QString("border-image: url(:/images/leftArrow.png);")
            + "width: 70;"
            + "height: 70;";

    ui->previousQuestionButton->setStyleSheet(qssText);
    timer = new QTimer(this);

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

        ui->testComboBox->addItem(finalName);
    }

    database.close();

    connect(timer, SIGNAL(timeout()), this, SLOT(update_timer()));
}

MainMenu::~MainMenu()
{
    delete ui;
}

QString MainMenu::crack_cipher(QString input)
{
    QString output = "";
    QString in = "ЙЦУКЕНГШЩЗХЪЭЖДЛОРПАВЫФЯЧСМИТЬБЮйцукенгшщзхъэждлорпавыфячсмитьбюQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890 ";
    QString out = "ЮБЬТИМСЧЯФЫВАПРОЛДЖЭЪХЗЩШГНЕКУЦЙюбьтимсчяфывапролджэъхзщшгнекуцйMNBVCXZLKJHGFDSAPOIUYTREWQmnbvcxzlkjhgfdsapoiuytrewq0987654321*";

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

void MainMenu::on_confirmButton_clicked()
{
    if(ui->surnameEdit->text().isEmpty())
    {
        return;
    }

    if(ui->testComboBox->currentText().isEmpty())
    {
        return;
    }


    currentTest = "";
    studentsName = ui->surnameEdit->text();

    for(int i = 0; i < ui->testComboBox->currentText().size(); i++)
    {
        if(ui->testComboBox->currentText().at(i) == "_")
        {
            currentTest += "_";
        }
        else
        {
            currentTest += ui->testComboBox->currentText().at(i);
        }
    }

    questions.clear();
    answers.clear();

    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(qApp->applicationDirPath() + "/tests/questions.db");
    database.open();

    QSqlQuery query(database);

    QString que = "SELECT * FROM "
            + currentTest;
    query.exec(que);

    while(query.next())
    {
        question buf;

        buf.questionText = crack_cipher(query.value(0).toString());
        buf.firstAnswer = crack_cipher(query.value(1).toString());
        buf.secondAnswer = crack_cipher(query.value(2).toString());
        buf.thirdAnswer = crack_cipher(query.value(3).toString());
        buf.fourthAnswer = crack_cipher(query.value(4).toString());
        buf.correctAnswer = crack_cipher(query.value(5).toString());

        questions.push_back(buf);
    }

    que = "SELECT * FROM settings_of_tables WHERE nameOfTable = '"
            + currentTest + "'";

    query.exec(que);

    if(query.next())
    {
        numberOfQuestions = query.value(2).toString().toInt();
        timeForTest = query.value(1).toInt();
    }

    timeForTest *= 60;

    while(questions.size() > numberOfQuestions)
    {
        int num = rand() % (questions.size() - 1);
        questions.erase(questions.begin() + num);
    }

    for(int i = 0; i < numberOfQuestions; i++)
    {
        int fIn = rand() % numberOfQuestions;
        int sIn = rand() % numberOfQuestions;
        question buf = questions.at(fIn);
        questions[fIn] = questions.at(sIn);
        questions[sIn] = buf;
    }

    database.close();

    for(int i = 0; i < numberOfQuestions; i++)
    {
        answers.push_back(" ");
    }

    timerTime = 0;
    timer->setInterval(1000);
    timer->start();

    update_timer();

    currentPage = 1;

    create_question();

    ui->stackedWidget->setCurrentIndex(1);
}

void MainMenu::update_timer()
{
    timerTime++;
    QString timeText = "Time Left: "
            + QString::number((timeForTest - timerTime) / 60)
            + QString(" m ")
            + QString::number((timeForTest - timerTime) % 60)
            + " s ";

    ui->timeLeft->setText(timeText);

    if(timeForTest - timerTime == 0)
    {
        timer->stop();
        end_testing();
    }
}

void MainMenu::create_question()
{
    ui->questionLabel->setText(questions.at(currentPage - 1).questionText);
    ui->firstAnswer->setText(questions.at(currentPage - 1).firstAnswer);
    ui->secondAnswer->setText(questions.at(currentPage - 1).secondAnswer);
    ui->thirdAnswer->setText(questions.at(currentPage - 1).thirdAnswer);
    ui->fourthAnswer->setText(questions.at(currentPage - 1).fourthAnswer);

    if(answers.at(currentPage-1)
            == questions.at(currentPage-1).firstAnswer)
    {
        ui->firstAnswer->setChecked(true);
        ui->secondAnswer->setChecked(false);
        ui->thirdAnswer->setChecked(false);
        ui->fourthAnswer->setChecked(false);
    }

    else if(answers.at(currentPage-1)
            == questions.at(currentPage-1).secondAnswer)
    {
        ui->firstAnswer->setChecked(false);
        ui->secondAnswer->setChecked(true);
        ui->thirdAnswer->setChecked(false);
        ui->fourthAnswer->setChecked(false);
    }

    else if(answers.at(currentPage-1)
            == questions.at(currentPage-1).thirdAnswer)
    {
        ui->firstAnswer->setChecked(false);
        ui->secondAnswer->setChecked(false);
        ui->thirdAnswer->setChecked(true);
        ui->fourthAnswer->setChecked(false);
    }

    else if(answers.at(currentPage-1)
            == questions.at(currentPage-1).fourthAnswer)
    {
        ui->firstAnswer->setChecked(false);
        ui->secondAnswer->setChecked(false);
        ui->thirdAnswer->setChecked(false);
        ui->fourthAnswer->setChecked(true);
    }
    else
    {
        ui->firstAnswer->setChecked(false);
        ui->secondAnswer->setChecked(false);
        ui->thirdAnswer->setChecked(false);
        ui->fourthAnswer->setChecked(false);
    }
}

void MainMenu::end_testing()
{
    ui->stackedWidget->setCurrentIndex(2);

    int count = 0;
    for(int i = 0; i < answers.size(); i++)
    {
        if(answers.at(i) == questions.at(i).correctAnswer)
        {
            count++;
        }
    }

    qDebug() << QString::number(count);
    double oneTask = 12.0 / numberOfQuestions;
    mark = count * oneTask;


    qDebug() << QString::number(oneTask);
    qDebug() << QString::number(mark);

    QPixmap originalImage(":/images/stars.png");
    ui->markLabel->setText(QString::number(mark));

    if(mark < 3)
    {
        QPixmap cropped = originalImage.copy(QRect(0, 0, 114, 20));
        ui->starsLabel->setPixmap(cropped);
    }
    else if(mark >= 3 && mark <= 5)
    {
        QPixmap cropped = originalImage.copy(QRect(0, 20, 114, 20));
        ui->starsLabel->setPixmap(cropped);
    }
    else if(mark >= 6 && mark <= 8)
    {
        QPixmap cropped = originalImage.copy(QRect(0, 40, 114, 20));
        ui->starsLabel->setPixmap(cropped);
    }
    else if(mark >=9 && mark <= 10)
    {
        QPixmap cropped = originalImage.copy(QRect(0, 61, 114, 20));
        ui->starsLabel->setPixmap(cropped);
    }
    else
    {
        QPixmap cropped = originalImage.copy(QRect(0, 82, 114, 20));
        ui->starsLabel->setPixmap(cropped);
    }
}

void MainMenu::on_nextQuestionButton_clicked()
{
    if(students_answer() != "")
    {
        answers[currentPage-1] = students_answer();
    }

    currentPage++;

    if(currentPage == questions.size() + 1)
    {
        currentPage = 1;
    }

    ui->numberQuestionLabel->setText(QString::number(currentPage));

    create_question();
}

QString MainMenu::students_answer()
{
    if(ui->firstAnswer->isChecked())
    {
        return ui->firstAnswer->text();
    }
    else if(ui->secondAnswer->isChecked())
    {
        return ui->secondAnswer->text();
    }
    else if(ui->thirdAnswer->isChecked())
    {
        return ui->thirdAnswer->text();
    }
    else if(ui->fourthAnswer->isChecked())
    {
        return ui->fourthAnswer->text();
    }
    else
    {
        return "";
    }
}

void MainMenu::on_previousQuestionButton_clicked()
{
    if(students_answer() != "")
    {
        answers[currentPage-1] = students_answer();
    }

    currentPage--;

    if(currentPage == 0)
    {
        currentPage = questions.size();
    }

    ui->numberQuestionLabel->setText(QString::number(currentPage));

    create_question();
}

void MainMenu::on_finishButton_clicked()
{
    end_testing();
}

void MainMenu::on_okEndButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainMenu::on_exitButton_clicked()
{
    exit(0);
}

void MainMenu::on_firstAnswer_clicked()
{
    ui->firstAnswer->setChecked(true);
    ui->secondAnswer->setChecked(false);
    ui->thirdAnswer->setChecked(false);
    ui->fourthAnswer->setChecked(false);
}

void MainMenu::on_secondAnswer_clicked()
{
    ui->firstAnswer->setChecked(false);
    ui->secondAnswer->setChecked(true);
    ui->thirdAnswer->setChecked(false);
    ui->fourthAnswer->setChecked(false);
}

void MainMenu::on_thirdAnswer_clicked()
{
    ui->firstAnswer->setChecked(false);
    ui->secondAnswer->setChecked(false);
    ui->thirdAnswer->setChecked(true);
    ui->fourthAnswer->setChecked(false);
}

void MainMenu::on_fourthAnswer_clicked()
{
    ui->firstAnswer->setChecked(false);
    ui->secondAnswer->setChecked(false);
    ui->thirdAnswer->setChecked(false);
    ui->fourthAnswer->setChecked(true);
}
