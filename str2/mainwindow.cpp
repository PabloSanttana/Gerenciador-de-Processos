#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <sys/types.h>
#include <QDebug>
#include <cmath>

QString path = "./";
QString file = "process.txt";
pid_t pid;
int linesTemp = 0;
QString command = "";
QString command_1 = "ps -ufa";
QString command_end = ">" + path + file;


MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList titles;

    titles  << "USER"  << "PID" << "PPID" << "PRI" <<"NI"<< "STATUS" << "%CPU" <<  "%MEM" << "START" << "TIME" << "NAME";
    ui->tableWidget->setColumnCount(11);
    ui->tableWidget->setHorizontalHeaderLabels(titles);
    ui->tableWidget->setColumnWidth(10,200);
    ui->tableWidget->setEditTriggers((QAbstractItemView::NoEditTriggers)); //
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->progressBar_0->setValue(0);
    ui->progressBar_1->setValue(0);
    ui->progressBar_2->setValue(0);
    ui->progressBar_3->setValue(0);
    ui->progressBar_4->setValue(0);
    ui->progressBar_5->setValue(0);
    ui->lineEdit_filtro->setText("");



    temp = new QTimer(this);
    tempProcess = new QTimer(this);

    connect(temp, SIGNAL(timeout()), this, SLOT(updateProcess()));
    connect(tempProcess, SIGNAL(timeout()), this, SLOT(process()));

    tempProcess ->start(950);
    temp->start(1000);


}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::verifyPID(QString pid)
{
    int rows = ui->tableWidget->rowCount();
    bool findboll = false;
    for(int i = 0; i < rows; ++i)
    {
        if(ui->tableWidget->item(i, 1)->text() == pid)
        {
            findboll = true;
            break;
        }
    }
    if(!findboll)
    {
        QMessageBox::warning(this, "Error", "PID não encontrado.");
    }

    return findboll;
}

void MainWindow::getCPUsPorcenta()
{
    QFile arquivo("./cpu.txt");
    if(!arquivo.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this, "Error", "Erro ao abrir arquivo");

    }
    QTextStream entrada(&arquivo);

    float cpus[6] = {0,0,0,0,0,0};
    bool notLineOne = false;

    while (!entrada.atEnd())
    {
        QString line = entrada.readLine().simplified();
        QStringList fields = line.split(" ");
        qInfo() << fields;
        if(notLineOne){
            if(fields.length() > 1){
                   int index = fields[0].toInt();
                   float val = fields[1].toFloat();
                   cpus[index] += val;
            }
        }else{
            notLineOne=true;
        }

    }

    ui->progressBar_0->setValue(ceil(cpus[0]));
    ui->progressBar_1->setValue(ceil(cpus[1]));
    ui->progressBar_2->setValue(ceil(cpus[2]));
    ui->progressBar_3->setValue(ceil(cpus[3]));
    ui->progressBar_4->setValue(ceil(cpus[4]));
    ui->progressBar_5->setValue(ceil(cpus[5]));

    arquivo.close();
}

void MainWindow::clearTable(int lines)
{

    while(ui->tableWidget->rowCount() > lines){
         qInfo() << "ROW 2" <<ui->tableWidget->rowCount();
           ui->tableWidget->removeRow(lines);
    }


}


void MainWindow::quit()
{
     exit(0);

}

void MainWindow::process()
{

    command = "";
    QString filter = ui->lineEdit_filtro->text();
    if(filter.length() > 1){
        qInfo() << filter;
        command = command_1 + "| grep " + filter + command_end;
    }else{
        command = "ps -uax" + command_end;
    }

    QString data(command);
    char *dataChar = data.toLocal8Bit().data();
    system(dataChar);
    system("ps a -o psr,pcpu > cpu.txt");
    getCPUsPorcenta();

}
void MainWindow::updateProcess()
{
    QFile arquivo(path+file);
    if(!arquivo.open(QFile::ReadOnly|QFile::Text)){
        QMessageBox::warning(this, "Error", "Erro ao abrir arquivo");

    }

    QTextStream entrada(&arquivo);

    int Max_Number_of_Lines = 0;

    while (!entrada.atEnd())
    {
        QString line = entrada.readLine().simplified();
        QStringList fields = line.split(" ");
        QStringList listStatus;
        QStringList OrbayFields;

       // qInfo()<< fields;

        //TebaleView
        //"USER PID PPID PRI NI STATUS %CPU %MEM START TIME NAME"

        if(fields[1] != "PID"){

            //return PPID PRI NI SATUS NAME

            listStatus = getStatusPID(fields[1]);

            //"USER PID PPID PRI NI STATUS %CPU %MEM START TIME NAME"

            if(listStatus.length() == 0){
                OrbayFields << fields[0] << fields[1] << ""
                        << "" << "" << ""
                        << fields[2] << fields[3] << fields[8] << fields[9] << fields[fields.length()-2] + fields[fields.length()-1];


            }else{
                OrbayFields << fields[0] << fields[1] << listStatus[0]
                        << listStatus[1] << listStatus[2] << listStatus[3]
                        << fields[2] << fields[3] << fields[8] << fields[9] << listStatus[4];
            }


            if(Max_Number_of_Lines >= ui->tableWidget->rowCount()){
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            }

           for (int column=0;column< OrbayFields.length();column++ ) {
                ui->tableWidget->setItem(Max_Number_of_Lines, column ,new QTableWidgetItem(OrbayFields[column]));
           }

        Max_Number_of_Lines++;
        }

    }

    clearTable(Max_Number_of_Lines);
    arquivo.close();

}

void MainWindow::processFilter()
{
    process();
    updateProcess();
}


void MainWindow::kill()
{

    QString kill_PID = ui->lineEdit_PID->text();
    qInfo() << kill_PID;
    if(verifyPID(kill_PID)){
        qInfo() << "MATA = " +  kill_PID;
        QString data("kill -9 "+ kill_PID);
        char *dataChar = data.toLocal8Bit().data();
        system(dataChar);
    }

       processFilter();
       ui->lineEdit_PID->setText("");
       ui->lineEdit_filtro->setText("");
}

void MainWindow::stop()
{
    QString stop_PID = ui->lineEdit_PID->text();
    if(verifyPID(stop_PID)){
        qInfo() << "STOP = " +  stop_PID;
        QString data("kill -19 "+ stop_PID);
        char *dataChar = data.toLocal8Bit().data();
        system(dataChar);
    }


}

void MainWindow::cont()
{
    QString count_PID = ui->lineEdit_PID->text();
    if(verifyPID(count_PID)){
        qInfo() << "count = " +  count_PID;
        QString data("kill -18 "+ count_PID);
        char *dataChar = data.toLocal8Bit().data();
        system(dataChar);
    }

}



void MainWindow::selectPID(int l, int c)
{
    QString PID = ui->tableWidget->item(l,1)->text();
    qInfo() <<PID;
    ui->lineEdit_PID->setText(PID);
}

void MainWindow::priorityProcess()
{
    QString pri_PID = ui->lineEdit_PID->text();
    QString pri = ui->lineEdi_Prioridade->text();

    if(pri.toInt() < -20 || pri.toInt() > 20 ){
         QMessageBox::warning(this, "Error", "intervalo incorreto: -20 a 20");
        return;
    }


    qInfo() << pri_PID;

    if(verifyPID(pri_PID)){
        qInfo() << "priorityProcess = " +  pri_PID;
         QString data("sudo renice " + pri + " -p " + pri_PID);
        char *dataChar = data.toLocal8Bit().data();
        system(dataChar);
    }
    ui->lineEdi_Prioridade->setText("");
}

void MainWindow::cpuAllocate()
{
//
        QString allocate_PID = ui->lineEdit_PID->text();
        QString CPUs = ui->lineEdit_CPU->text();
        qInfo() << CPUs;
        if(verifyPID(allocate_PID)){

            qInfo() << "priorityProcess = " +  allocate_PID;
            QString cmd = "taskset -pc "+ CPUs + " "+ allocate_PID;
            qInfo() << cmd;
             QString data(cmd);
            char *dataChar = data.toLocal8Bit().data();
            system(dataChar);
        }
}

QStringList MainWindow::getStatusPID(QString PID)
{

   // qInfo()<< "/proc/"+PID+"/stat";

    QFile file("/proc/"+PID+"/stat");

    QStringList infoProcess;
    QString status;
    if(file.open(QFile::ReadOnly)){

        QString line = file.readAll().simplified();
        QStringList fields = line.split(' ');
        //qInfo() << fields;
        //qInfo() << "name = " << fields[1];
        //qInfo() << "State = " << fields[2];
        //qInfo() << "PPid = " << fields[3];
        //qInfo() << "PRI = " << fields[17];
        //qInfo() << "NI = " << fields[18];

        if(fields[2] == "S"){
           status = "S (sleeping)";
        }else if(fields[2] == "T"){
             status = "T (stopped)";
        }else{
             status = "R (running)";
        }

        infoProcess <<  fields[3] << fields[17] << fields[18] << status << fields[1];
        file.close();
    }

     return infoProcess;
}



























