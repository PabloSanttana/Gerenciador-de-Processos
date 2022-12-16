#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setRouteTrain_y();
    void setRouteTrain_b();
    void setRouteTrain_r();


public slots:
    void positionTrain_y(int x);
    void positionTrain_b(int x);
    void positionTrain_r(int x);
    void VelocidadeY(int x);
    void VelocidadeR(int x);
    void VelocidadeB(int x);


signals:
   void setVelocidadeY(int x);
   void setVelocidadeR(int x);
   void setVelocidadeB(int x);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
