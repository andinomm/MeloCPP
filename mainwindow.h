#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <vector>

class QTabWidget;
class QTextEdit;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_action_Save_triggered();

    void on_action_Build_triggered();

    void on_actionSave_As_triggered();

    void on_actionBuild_and_Run_triggered();

    void on_actionRun_triggered();

    void on_actionNew_File_triggered();

    void on_actionCompiler_triggered();

    void on_action_Open_triggered();

    void on_text_Changed();


private:
    Ui::MainWindow *ui;
    QString savingLocation, compilerPath, buildLocation;
    QTextEdit *textEdit;
    bool savedState;


    QString convertPathsToArgs(const QString &path);

    void findCompiler();
};

#endif // MAINWINDOW_H
