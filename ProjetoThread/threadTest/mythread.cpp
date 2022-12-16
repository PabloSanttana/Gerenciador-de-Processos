#include "mythread.h"
#include <QtDebug>



Mythread::Mythread()
{

}

void Mythread::init(char train, pthread_mutex_t *rail_1, pthread_mutex_t *rail_2)
{
    this->_rail_1 = rail_1;
    this->_rail_2 = rail_2;
    this->_train = train;
}


void Mythread::setVelocidade(int x)
{
    this->vlec = (1100 - (x*10));
}

void Mythread::run()
{
    if(this->_train == 'R'){
        trainR();
    }else if(this->_train == 'Y'){
        trainY();
    }else if(this->_train == 'B'){
        trainB();
    }

}

void Mythread::trainY()
{
    while (1) {
        if(!this->isFinished()){
            qInfo("trainY -- Dentro da região Livre \n");
            for(int i=0; i < 4;i++){
                  emit trainPosition(i);
                 msleep(this->vlec);
            }
             pthread_mutex_lock(_rail_1);
             msleep(30);
             qInfo("trainY -- Dentro da região crítica _rail_1 \n");
             for(int i=4; i < 7;i++){
                  emit trainPosition(i);
                  msleep(this->vlec);
             }

             pthread_mutex_lock(_rail_2);
              qInfo("trainY -- Dentro da região crítica 2 \n");
             pthread_mutex_unlock(_rail_1);
             qInfo("trainY -- Fora da região crítica 1 \n");

             for(int i=7; i < 10;i++){
                  emit trainPosition(i);
                  msleep(this->vlec);

             }
             pthread_mutex_unlock(_rail_2);

             qInfo("trainY -- Fora da região crítica _rail_2 \n");

             for(int i=10; i < 13;i++){
                  emit trainPosition(i);
                  msleep(this->vlec);
             }

        }else{
            this->exec();
        }
    }
}
void Mythread::trainB()
{
    while (1) {
        if(!this->isFinished()){
            qInfo("trainB -- Dentro da região Livre \n");
            for(int i=0; i < 7;i++){
                  emit trainPosition(i);
                  msleep(this->vlec);
            }
             pthread_mutex_lock(_rail_1);
             msleep(30);
             qInfo("trainB -- Dentro da região crítica 1 \n");
             for(int i=7; i < 10;i++){
                  emit trainPosition(i);
                  msleep(this->vlec);
             }

             pthread_mutex_lock(_rail_2);
             qInfo("trainB -- Dentro da região crítica 2 \n");
             pthread_mutex_unlock(_rail_1);
             qInfo("trainB -- Fora da região 1 \n");

             for(int i=10; i < 13;i++){
                  emit trainPosition(i);
                  msleep(this->vlec);

             }
             pthread_mutex_unlock(_rail_2);
             qInfo("trainB -- Fora da região crítica 2 \n");

        }else{
            this->exec();
        }
    }
}
void Mythread::trainR()
{
    while (1) {
        if(!this->isFinished()){
            qInfo("trainR -- Dentro da região Livre \n");
            for(int i=0; i < 13;i++){
                 emit trainPosition(i);
                  msleep(this->vlec);;
            }

             pthread_mutex_lock(_rail_2);
             pthread_mutex_lock(_rail_1);

             qInfo("trainR -- Dentro da região críticas \n");

             for(int i=12; i < 15;i++){
                 emit trainPosition(i);
                 msleep(this->vlec);
             }

            for(int i=15; i < 19;i++){
                emit trainPosition(i);
                msleep(this->vlec);
                if(i==15){
                     pthread_mutex_unlock(_rail_1);
                     qInfo("trainR -- Fora da região crítica 1 \n");
                }

            }
             pthread_mutex_unlock(_rail_2);
             qInfo("trainR -- Fora da região crítica 2 \n");

        }else{
            this->exec();
        }
    }
}
