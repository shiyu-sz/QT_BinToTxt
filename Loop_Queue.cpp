/*
 * Loop_Queue.c
 *
 *  Created on: 2016年8月27日
 *      Author: sy
 */

#include <stdio.h>
#include <stddef.h>
#include "Loop_Queue.h"
#include "QDebug"
#include "string.h"

Queue_Data gQueue_Data;

//设置队列为空
//sq:队列名
void QueueSetNull(Queue_Data *sq)
{
	sq->front = 0;
	sq->rear = 0;
    memset(sq, 0x00, sizeof(Queue_Data));
}

//获取头结点
//sq:队列名
//返回值:获取到的节点数据，失败返回空
Data_Node * QueueGetFront(Queue_Data *sq)
{
	if(sq->rear == sq->front)	//判断队列是否为空
	{
        qDebug("QueueGetFront Warning : queue is null!\n");
		return (Data_Node *)NULL;
	}
	else
		return &sq->data[((sq->front+1) % QUEUE_NODE_MAX)];
}

//入队列
//sq:队列名
//src_data:要入队列的数据
//返回佱：0-入队列失败 1-入队列成功
int QueueInDataNode(Queue_Data *sq, Data_Node src_data)
{
	if((sq->rear + 1) % QUEUE_NODE_MAX == sq->front)	//判断队列是否已满
	{
        qDebug("sq->front = %d, sq->rear = %d", sq->front, sq->rear);
//		qDebug("QueueInDataNode Warning : queue is full!\n");
        return 0;
	}
	else
	{
        sq->rear = (sq->rear + 1) % QUEUE_NODE_MAX;		//尾指针后移
        strncpy(sq->data[sq->rear].InputFilePath, src_data.InputFilePath, FILE_DIR_LEN);
        strncpy(sq->data[sq->rear].OutputFilePath, src_data.OutputFilePath, FILE_DIR_LEN);
		return 1;		//入队成功返回1
	}
}

//出队列
//sq:队列名
//返回值:获取到的节点数据，失败返回空
Data_Node * QueueOutDataNode(Queue_Data *sq)
{
	if(sq->rear == sq->front)		//判断队列是否为空
	{
//        qDebug("sq->rear = %d, sq->front = %d", sq->rear, sq->front);
//		qDebug("QueueOutDataNode Warning : queue is null!\n");
		return (Data_Node *)NULL;
	}
	else
	{
		sq->front = (sq->front + 1) % QUEUE_NODE_MAX;	//头指针后移
		return &sq->data[sq->front];					//返回头结点的值
	}
}




