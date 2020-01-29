#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QTextStream>
#include <QProcess>
#include <QIODevice>
#include<QTabWidget>
#include<QTextEdit>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    savingLocation(""),
    compilerPath("C:\\Program Files (x86)\\OJI\\CodeBlocks\\MinGW\\bin\\g++.exe"),
    buildLocation(""),
    textEdit(nullptr),
    savedState(false)

{


    findCompiler();

    ui->setupUi(this);
    ui->imgLabel->setPixmap(ui->imgLabel->pixmap()->scaled(300,184, Qt::KeepAspectRatio));
    QString aux = convertPathsToArgs(buildLocation);
}

MainWindow::~MainWindow()
{
    delete ui;


}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::findCompiler()
{

    QFile openFile("compilerConf.txt");
    if(!openFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Eroare la citire compilerConf!";
    }

    QTextStream in(&openFile);
    compilerPath = in.readLine();
    openFile.close();

   // qDebug() << compilerPath;

}



void MainWindow::on_action_Save_triggered()   //functia de save
{

    if(savingLocation!=""){

    QFile fisierul(savingLocation);
    fisierul.open(QIODevice::WriteOnly | QIODevice::Text);


    QTextStream OS(&fisierul);
    if(!fisierul.isOpen()){
        qDebug() << "Error";
    }
    OS << textEdit->toPlainText();
    fisierul.close();

    savedState = true;
    }


    else
    {
        on_actionSave_As_triggered();
    }
}

void MainWindow::on_action_Build_triggered()
{

    on_action_Save_triggered();


    if(buildLocation == "")
    {
        qDebug() << "buildLocation not specified";
    }




    QStringList argumentsProgram;


    argumentsProgram << "-o" << convertPathsToArgs(buildLocation) << convertPathsToArgs(savingLocation)  ;

   // qDebug() << convertPathsToArgs(buildLocation);
   // qDebug() << convertPathsToArgs(savingLocation);

    //qDebug() << compilerPath.toStdString().c_str();


    QProcess *builtCodeProgram = new QProcess();
    builtCodeProgram->start(compilerPath, argumentsProgram);


    if(builtCodeProgram->waitForStarted() == false)
    {
        qDebug() << "Failed to start!";
        builtCodeProgram->kill();
    }



    if(builtCodeProgram->waitForFinished(15000) == false)
    {
        qDebug() << "FAILED TO FINISH PROGRAM!!";
        builtCodeProgram->kill();
    }


    builtCodeProgram->setReadChannel(QProcess::StandardOutput);
    qDebug() << builtCodeProgram->readAllStandardOutput();


    delete builtCodeProgram;


}

void MainWindow::on_actionSave_As_triggered()
{
    QString newLocation;
    newLocation = QFileDialog::getSaveFileName(this,
                                                "Save file",
                                                QDir::currentPath(),"C++ File (*.cpp);;All Files(*.*)");

    if(newLocation!=""){
        savingLocation = newLocation;
        buildLocation = newLocation;
        buildLocation.replace(".cpp", ".exe");
       // qDebug() << buildLocation << " " << savingLocation;
        on_action_Save_triggered();
    }
}

void MainWindow::on_actionBuild_and_Run_triggered()
{
    on_action_Build_triggered();
    on_actionRun_triggered();
}

void MainWindow::on_actionRun_triggered()
{

    QByteArray ba = convertPathsToArgs(buildLocation).toLatin1();
  //  qDebug() << convertPathsToArgs(buildLocation);


    system(ba);


}

QString MainWindow::convertPathsToArgs(const QString &path)
{
    QString aux = path;
    aux.replace("/", "\\");
    return aux;

}

void MainWindow::on_actionNew_File_triggered()
{

    if(ui->mainHLayout != nullptr)
    {
        delete ui->mainHLayout;
        ui->mainHLayout = nullptr;
    }

    if(ui->descriptionLabel != nullptr)
    {
        delete ui->descriptionLabel;
        ui->descriptionLabel = nullptr;
    }

    if(textEdit != nullptr)
    {
        QMessageBox::StandardButton newFileAlert = QMessageBox::question(this, "Are you sure?", "Are you sure you want another file?\n Your unsaved work will be lost.", QMessageBox::Yes|QMessageBox::No);

        if(newFileAlert == QMessageBox::No)
            return;
        else {delete textEdit; textEdit=nullptr;}

    }

    textEdit = new QTextEdit();
    savedState = false;

    connect(textEdit,SIGNAL(textChanged()),this,SLOT(on_text_Changed()));
    QFont f( "Courier New", 20, QFont::Courier);

    textEdit->setFont(f);
    ui->verticalLayout->addWidget(textEdit);
    ui->actionBuild_and_Run->setEnabled(true);
    ui->action_Build->setEnabled(true);
    ui->actionRun->setEnabled(true);

    savingLocation="";
    buildLocation="";

}

void MainWindow::on_text_Changed()
{

    savedState = false;
}


void MainWindow::on_actionCompiler_triggered()
{
    QString newLocation = QFileDialog::getOpenFileName(this,
                                                "Choose Compiler",
                                                QDir::currentPath(),"Executable File (*.exe);;All Files(*.*)");

    if(newLocation != "")
    {
        QFile fisierul("compilerConf.txt");




        if(!fisierul.open(QIODevice::WriteOnly | QIODevice::Text))
        {qDebug() << "Error opening!!";}


        QTextStream OS(&fisierul);
        if(!fisierul.isOpen()){
            qDebug() << "Error writing compilerConf";
        }
        OS << convertPathsToArgs(newLocation);
        fisierul.close();
     }



}

void MainWindow::on_action_Open_triggered()
{

    QString openfileLocation = QFileDialog::getOpenFileName(this,
                                                "Choose File",
                                                QDir::currentPath(),"C++ Files (*.cpp);;All Files(*.*)");

    if(openfileLocation == "")
        return;



    on_actionNew_File_triggered();



    QFile openFile(openfileLocation);
    if(!openFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Eroare la citire!";
    }

    QTextStream in(&openFile);

    while(!in.atEnd()) {
        QString line = in.readLine();
        textEdit->setText(textEdit->toPlainText()  + line + "\n");
    }

    openFile.close();

    savingLocation = openfileLocation;
    buildLocation =  openfileLocation.replace(".cpp", ".exe");


    qDebug() << savingLocation << " " <<buildLocation;


}
