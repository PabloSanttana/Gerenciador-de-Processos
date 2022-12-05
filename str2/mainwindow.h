#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool verifyPID(QString pid);
    void getCPUsPorcenta();
    void clearTable(int lines);
    QStringList getStatusPID(QString PID);

public slots:
    void quit();
    void process();
    void updateProcess();
    void processFilter();
    void kill();
    void stop();
    void cont();
    void selectPID(int l, int c);
    void priorityProcess();
    void cpuAllocate();


private:
    Ui::MainWindow *ui;

    QTimer *temp;
    QTimer *tempProcess;
};
#endif // MAINWINDOW_H
