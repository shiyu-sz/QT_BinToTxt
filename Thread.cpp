
#include "Thread.h"
#include "main.h"
#include "file_operation.h"
#include <QFile>
#include <QDebug>
#include "Loop_Queue.h"

//写文件线程初始化
textTread::textTread(QObject *parent) :
    QThread(parent)
{
    qDebug() << "Thread Init!" ;
    file_buff = (short int *)malloc(FILE_SIZE);
}

//
void textTread::run()
{
    Data_Node * outdata;
    unsigned int i;
    Data_Head_Info file_head;
    while(1)
    {
        switch(gCurrent_cmd)
        {
            case NO_INSTRUCT:
            {
                msleep(100); break;
            }
            case START_CONVERSION:
            {
                outdata = QueueOutDataNode(&gQueue_Data);
                if(outdata == (Data_Node *)NULL)
                {
                    gCurrent_cmd = CONVERSION_FINISH;
                    break;
                }

                qDebug() << "Thread inputfile" << outdata->InputFilePath << endl;
                qDebug() << "Thread outputfile" << outdata->OutputFilePath << endl;

                read_file_f(outdata->InputFilePath, file_buff, sizeof(short int), (char *)&file_head, sizeof(char), sizeof(Data_Head_Info));

                QDateTime current_date_time =QDateTime::currentDateTime();
                QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
                qDebug() << current_date << endl;

                QFile f(QString(QLatin1String(outdata->OutputFilePath)));
                if(!(f.open(QIODevice::WriteOnly | QIODevice::Text)))
                {
                    qDebug() << "Open failed" << OutputFile;
                }
                QTextStream txtInput(&f);
                for(i = 0; i < file_head.data_len/sizeof(short int); i ++)
                {
                    txtInput << file_buff[i] << endl;
                }
                f.close();
                emit signal_updata_state(done_number + 1, total_number, outdata);
                i = 0;
                done_number ++;
                qDebug() << "ssss" << endl;
                break;
            }
            case CONVERSION_FINISH:
            {
                msleep(100);
                break;
            }
        }
        msleep(100);
    }
}

void textTread::GetGUIThreadID(int mThreadID)  //接受主线程通讯过来的数据
{
    ThreadID = mThreadID;
    qDebug() << tr("Thread is Open,ThreadID : %1").arg(ThreadID);
}
