#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mythread.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <QtDebug>

Mythread *train_y,*train_r,*train_b;

pthread_mutex_t shared_rail_1,shared_rail_2,shared_rail_3;

QList<QLabel*> route_train_y,route_train_r,route_train_b;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setRouteTrain_y();
    setRouteTrain_b();
    setRouteTrain_r();


    pthread_mutex_init(&shared_rail_1, NULL);
    pthread_mutex_init(&shared_rail_2, NULL);
    pthread_mutex_init(&shared_rail_3, NULL);

    // trem amarelo
    train_y = new Mythread();
    train_y->init('Y',&shared_rail_1,&shared_rail_2);

    // trem azul
    train_b = new Mythread();
    train_b->init('B',&shared_rail_3,&shared_rail_1);

    // trem red
    train_r = new Mythread();
    train_r->init('R',&shared_rail_2,&shared_rail_3);

    connect(train_y, &Mythread::trainPosition, this, &MainWindow::positionTrain_y);
    connect(train_b, &Mythread::trainPosition, this, &MainWindow::positionTrain_b);
    connect(train_r, &Mythread::trainPosition, this, &MainWindow::positionTrain_r);

    connect(this, &MainWindow::setVelocidadeY,train_y, &Mythread::setVelocidade);
    connect(this, &MainWindow::setVelocidadeB,train_b, &Mythread::setVelocidade);
    connect(this, &MainWindow::setVelocidadeR,train_r, &Mythread::setVelocidade);


    train_y->start();
    train_b->start();
    train_r->start();

}

MainWindow::~MainWindow()
{
    delete  train_y;
    delete  train_b;
    delete  train_r;
    pthread_mutex_destroy(&shared_rail_1);
    pthread_mutex_destroy(&shared_rail_2);
    pthread_mutex_destroy(&shared_rail_3);
    delete ui;
}

void MainWindow::setRouteTrain_y()
{
    // livre
    route_train_y.append(ui->y_0);
    route_train_y.append(ui->y_1);
    route_train_y.append(ui->y_2);
    route_train_y.append(ui->y_3);

    // zona 1
    route_train_y.append(ui->rail1_0);
    route_train_y.append(ui->rail1_1);
    route_train_y.append(ui->rail1_2);

    // zona 2
    route_train_y.append(ui->rail2_2);
    route_train_y.append(ui->rail2_1);
    route_train_y.append(ui->rail2_0);

    // livre
    route_train_y.append(ui->y_4);
    route_train_y.append(ui->y_5);
    route_train_y.append(ui->y_6);


    for (int i=0; i < route_train_y.length();i++ ) {
        route_train_y[i]->setStyleSheet("background: white");
    }
}

void MainWindow::setRouteTrain_b()
{
    // livre
    route_train_b.append(ui->b_0);
    route_train_b.append(ui->b_1);
    route_train_b.append(ui->b_2);
    route_train_b.append(ui->b_3);
    route_train_b.append(ui->b_4);
    route_train_b.append(ui->b_5);
    route_train_b.append(ui->b_6);

    // fona 1 rail3_3
    route_train_b.append(ui->rail3_2);
    route_train_b.append(ui->rail3_1);
    route_train_b.append(ui->rail3_0);

    //zona 2
    route_train_b.append(ui->rail1_2);
    route_train_b.append(ui->rail1_1);
    route_train_b.append(ui->rail1_0);

    for (int i=0; i < route_train_b.length();i++ ) {
        route_train_b[i]->setStyleSheet("background: white");
    }
}

void MainWindow::setRouteTrain_r()
{

    //livre
    route_train_r.append(ui->r_0);
    route_train_r.append(ui->r_1);
    route_train_r.append(ui->r_2);
    route_train_r.append(ui->r_3);
    route_train_r.append(ui->r_4);
    route_train_r.append(ui->r_5);
    route_train_r.append(ui->r_6);
    route_train_r.append(ui->r_7);
    route_train_r.append(ui->r_8);
    route_train_r.append(ui->r_9);
    route_train_r.append(ui->r_10);
    route_train_r.append(ui->r_11);
    route_train_r.append(ui->r_12);


    // zona 1
    route_train_r.append(ui->rail2_0);
    route_train_r.append(ui->rail2_1);
    route_train_r.append(ui->rail2_2);

    // zona 2

    route_train_r.append(ui->rail3_0);
    route_train_r.append(ui->rail3_1);
    route_train_r.append(ui->rail3_2);


    for (int i=0; i < route_train_r.length();i++ ) {
        route_train_r[i]->setStyleSheet("background: white");
    }
}

void MainWindow::positionTrain_y(int x)
{

      qInfo()<< "positionTrain_y " << x;
     if(x == 0){
       route_train_y[route_train_y.length()-1]->setStyleSheet("background: white");
       route_train_y[0]->setStyleSheet("background: yellow");

     }else{
         route_train_y[x]->setStyleSheet("background: yellow");
         route_train_y[x-1]->setStyleSheet("background: white");

     }
}

void MainWindow::positionTrain_b(int x)
{

    qInfo()<< "positionTrain_b :" << x;
    if(x == 0){
      route_train_b[route_train_b.length()-1]->setStyleSheet("background: white");
      route_train_b[0]->setStyleSheet("background: blue");

    }else{

        route_train_b[x]->setStyleSheet("background: blue");
         route_train_b[x-1]->setStyleSheet("background: white");

    }

}

void MainWindow::positionTrain_r(int x)
{
    qInfo()<< "positionTrain_b :" << x;
    if(x == 0){
      route_train_r[route_train_r.length()-1]->setStyleSheet("background: white");
      route_train_r[0]->setStyleSheet("background: red");

    }else{
        route_train_r[x]->setStyleSheet("background: red");
        route_train_r[x-1]->setStyleSheet("background: white");

    }
}

void MainWindow::VelocidadeY(int x)
{
    emit setVelocidadeY(x);
}

void MainWindow::VelocidadeR(int x)
{

    emit setVelocidadeR(x);
}

void MainWindow::VelocidadeB(int x)
{

    emit setVelocidadeB(x);
}


