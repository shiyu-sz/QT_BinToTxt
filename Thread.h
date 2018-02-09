#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include "Loop_Queue.h"

//写文件线程
class textTread : public QThread
{
    Q_OBJECT
public:
    textTread(QObject *parent = 0);

protected:
    void run();
signals:
    void signal_updata_state(unsigned int, unsigned int, Data_Node *);
public slots:
    void GetGUIThreadID(int mThreadID);

private:
    int ThreadID;
    short int * file_buff;
};

#endif // THREAD_H
