#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QList>
#include <QTimer>
#include <QVector>
#include <QPixmap>

struct question
{
    QString questionText;
    QString firstAnswer;
    QString secondAnswer;
    QString thirdAnswer;
    QString fourthAnswer;
    QString correctAnswer;
};

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = 0);
    ~MainMenu();

public slots:
    static QString create_cipher(QString input);

    static QString crack_cipher(QString input);

private slots:
    void on_confirmButton_clicked();

    void end_testing();

    void update_timer();

    void create_question();

    void on_nextQuestionButton_clicked();

    QString students_answer();

    void on_previousQuestionButton_clicked();

    void on_finishButton_clicked();

    void on_okEndButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::MainMenu *ui;

    QString studentsName = "";

    QList<question> questions;
    QVector<QString> answers;

    QString currentTest = "";

    int numberOfQuestions, timeForTest;
    int timerTime = 0;
    int mark = 6;

    QTimer *timer;
};

#endif // MAINMENU_H
