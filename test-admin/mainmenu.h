#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QListWidgetItem>
#include <QMessageBox>

#include "addingtable.h"
#include "settingstable.h"

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

    void update();

    void save_settings_for_table(QString time, QString numberOfQuestionsForStudents);

private slots:
    void on_loginButton_clicked();

    void on_addTableButton_clicked();

    void update_table_list();

    void update_question_list();

    void on_listOfTables_itemDoubleClicked(QListWidgetItem *item);

    void on_deleteTableButton_clicked();

    void on_firstAnswerCorrect_clicked();

    void on_secondAnswerCorrect_clicked();

    void delete_all_radios();

    void on_thirdAnswerCorrect_clicked();

    void on_fourthAnswerCorrect_clicked();

    void on_resetButton_clicked();

    void on_backButton_clicked();

    void clear_adding_question();

    void on_saveQuestionButton_clicked();

    QString create_cipher(QString input);

    QString crack_cipher(QString input);

    void on_editQuestionButton_clicked();

    void on_addQuestionButton_clicked();

    void on_deleteQuestionButton_clicked();

    void on_listOfQuestions_itemDoubleClicked(QListWidgetItem *item);

    void on_settingsButton_clicked();

private:
    Ui::MainMenu *ui;

    void add_passwords();

    QStringList passwords;

    QString currentTable = "";
    QString currentQuestion = "";

    bool isQuestionNew = true;

    QString in = "йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮQWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890 ";
    QString out = "юбьтимсчяэждлорпавыфъхзщшгнекуцйЮБЬТИМСЧЯЭЖДЛОРПАВЫФЪХЗЩШГНЕКУЦЙMNBVCXZLKJHGFDSAPOIUYTREWQmnbvcxzlkjhgfdsapoiuytrewq0987654321*";

signals:
    void emit_update_table();

    void emit_update_questions();

    void emit_delete_all_radios();

    void emit_clear_adding_question();
};

#endif // MAINMENU_H
