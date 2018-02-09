
#include <stdio.h>
#include <QDebug>
#include <QFile>
#include "Loop_Queue.h"

//从文件读数据
//file_name：文件名和路径
//data：数据指针
//data_len：数据类型的长度（字节）
//data_num：数据的项数
//返回值：0-读失败 1-读成功
int read_file_f(char * file_name, short int * data, int data_len, char * head, int head_len, int head_num)
{
    FILE * file;
    int res;
    int indatanum;
    Data_Head_Info temp_head;
    file = fopen(file_name , "rb");
    if(file == NULL)
    {
        qDebug("Warning : file open faile!\n");
        return (int)NULL;
    }

    indatanum = fread(head , head_len , head_num , file);
    memcpy(&temp_head, head, sizeof(Data_Head_Info));
    indatanum = fread(data , data_len , temp_head.data_len/data_len , file);
    res = fclose(file);
    if(res != 0)
        qDebug("Warning : close file error!\n");
    return indatanum;
}

//写采样数据到文件
int write_data_file(QString file, const short int *data, int len)
{
    QFile f(file);
    if(!(f.open(QIODevice::WriteOnly | QIODevice::Text)))
    {
        qDebug() << "Open failed" << file;
        return 0;
    }
    QTextStream txtInput(&f);

    for(int i = 0; i < len; i ++)
    {
        txtInput << data[i] << endl;
    }

    f.close();
    return 1;
}
