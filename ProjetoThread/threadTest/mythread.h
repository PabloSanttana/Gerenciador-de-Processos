#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QObject>
#include <QList>
#include <QLabel>

class Mythread : public  QThread
{
    Q_OBJECT
public:
    Mythread();
    void init(char train,pthread_mutex_t *rail_1, pthread_mutex_t *rail_2);
    void setPositionTrain(int index);
public slots:
    void setVelocidade(int x);
 signals:
    void trainPosition(int x);
private:
    char _train;
    int vlec = 1200;
    pthread_mutex_t *_rail_1, *_rail_2;
    void run();
    void trainY();
    void trainB();
    void trainR();
};

#endif // MYTHREAD_H
