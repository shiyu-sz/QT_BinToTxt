/*
 * Loop_Queue.h
 *
 *  Created on: 2016年8月27日
 *      Author: sy
 */

#ifndef LOOP_QUEUE_H_
#define LOOP_QUEUE_H_

#define QUEUE_NODE_MAX ((102400)+1)
#define FILE_DIR_LEN 50

#include <QDateTime>

//数据文件头
typedef struct
{
    time_t time;                    //时间
    unsigned int data_len;          //数据的长度(byte)
    unsigned int sample_multiple;     //采样档位
    unsigned int sampling_freq;     //采样频率(HZ)
    unsigned short int Gear;        //档位
    float Software_Multiple;        //软件倍数
    short int excursion;            //偏移
    float Capacity;                 //最大量程
}Data_Head_Info;

//数据的信息
typedef struct
{
    char InputFilePath[FILE_DIR_LEN];    //输入文件路径
    char OutputFilePath[FILE_DIR_LEN];    //输出文件路径
}Data_Node;

//数据信息的队列
typedef struct
{
    Data_Node data[QUEUE_NODE_MAX];		//队列中的元素
    short int front;		//队列的头结点
    short int rear;			//队列的尾结点
}Queue_Data;

//设置队列为空
void QueueSetNull(Queue_Data *sq);
//获取头结点
Data_Node * QueueGetFront(Queue_Data *sq);
//入队列
int QueueInDataNode(Queue_Data *sq, Data_Node src_data);
//出队列
Data_Node * QueueOutDataNode(Queue_Data *sq);

extern Queue_Data gQueue_Data;

#endif /* LOOP_QUEUE_H_ */
