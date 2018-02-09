#ifndef MAIN_H
#define MAIN_H

#include <QSemaphore>

#define FILE_SIZE (32*1024*1024)    //每个文件的大小，单位：KByte

#define FALSE 0
#define TRUE 1

//当前状态
enum e_Current_cmd
{
    NO_INSTRUCT,        //没有任务
    START_CONVERSION,   //开始转换
    CONVERSION_FINISH,   //转换完成
};

extern e_Current_cmd gCurrent_cmd;
extern QString InputFile;
extern QString OutputFile;
extern bool is_start; //TRUE
extern bool is_stop;
//extern short int * file_buff;
extern unsigned int total_number;
extern unsigned int done_number;
extern unsigned int frequency;
extern unsigned int multiplying;

#endif // MAIN_H
