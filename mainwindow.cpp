#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QSemaphore>
#include "Thread.h"
#include "main.h"
#include "Loop_Queue.h"
#include "showcpumemory.h"
#include "showdatetime.h"

bool ok;
e_Current_cmd gCurrent_cmd;
QString InputFile;
QString OutputFile;
//short int * file_buff;
unsigned int total_number = 0;
unsigned int done_number = 0;
unsigned int frequency = 0;
unsigned int multiplying = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //显示系统时间
    ShowDateTime *showDateTime = new ShowDateTime(this);
    showDateTime->SetLab(ui->label_MainWindow_ShowTime, ui->label_run_time);
    showDateTime->Start(1000);

    //显示CPU和内存占用
    ShowCPUMemory *showCPUMemory = new ShowCPUMemory(this);
    showCPUMemory->SetLab(ui->label_sysset_cpu);

    qDebug("Data_Head_Info = %d", sizeof(Data_Head_Info));

    ui->progressBar->setFormat(tr("未开始转换"));
    ui->progressBar->setValue(0);

//    pWriteFile = new WriteFileThread(this);
//    pWriteFile->start();

//    textTread * pWriteFile;
    pWriteFile = new textTread[3];
    for(int i=0;i<3;i++)
    {
        connect(this,SIGNAL(SendGUIThreadID(int)),&pWriteFile[i],SLOT(GetGUIThreadID(int)));
        connect(&pWriteFile[i], SIGNAL(signal_updata_state(unsigned int, unsigned int, Data_Node *)), this, SLOT(slots_updata_state(unsigned int, unsigned int, Data_Node *)));
        emit SendGUIThreadID(i);
        pWriteFile[i].start();
    }

//    file_buff = (short int *)malloc(FILE_SIZE);
}

MainWindow::~MainWindow()
{
//    free(file_buff);
    delete ui;
}

//遍历一个目录，找出目录下的所有文件
int traverse_dir(QString path)
{
    Data_Node tempdata;
    QString tempoutput;

    QDir dir(path);
    if(!dir.exists())
    {
        return 0;
    }

    QStringList nameFilters;
    nameFilters << "*" << "*";
    QFileInfoList files = dir.entryInfoList(nameFilters, QDir::NoDotAndDotDot|QDir::Dirs|QDir::Files|QDir::Readable, QDir::Name);

    for(int i = 0 ; i < files.length() ; i ++)
    {
        if( files[i].isDir() )
        {
//            qDebug() << "dir" << files[i].absoluteFilePath() << endl;
            traverse_dir(files[i].absoluteFilePath());
        }
        else if( files[i].isFile() )
        {
            //输入文件路径加入队列
            QByteArray ba = files[i].absoluteFilePath().toLatin1();
            char * ch = ba.data();
            strcpy( tempdata.InputFilePath, ch );
            //输出文件路径加入队列
            tempoutput = OutputFile + (files[i].absoluteFilePath().mid(InputFile.length())) + ".txt";

            QString sPath = tempoutput.left(tempoutput.lastIndexOf('/'));
            QDir dir(sPath);
            if(!dir.exists(sPath))
            {
                //路径不存在，则创建路径
                if(!dir.mkpath(sPath))
                {
                    qDebug() << "inputfile error" << endl;
                }
            }

            QByteArray ba1 = tempoutput.toLatin1();
            char * ch1 = ba1.data();
            strcpy( tempdata.OutputFilePath, ch1 );

            QueueInDataNode(&gQueue_Data, tempdata);
            total_number ++;

            qDebug() << "inputfile" << tempdata.InputFilePath << endl;
            qDebug() << "outputfile" << tempdata.OutputFilePath << endl;
        }
    }
    return 1;
}

//打开目录
void MainWindow::on_pushButton_open_clicked()
{
    InputFile = QFileDialog::getExistingDirectory(this, tr("Open Directory"),".",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_InputFile->setText(InputFile);
    if(InputFile.length() != 0)
    {
        qDebug() << "InputFile" << InputFile << endl;
    }

}

//保存目录
void MainWindow::on_pushButton_save_clicked()
{
    OutputFile = QFileDialog::getExistingDirectory(this, tr("Open Directory"),".",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_OutputFile->setText(OutputFile);
    if(!OutputFile.isNull())
    {
        qDebug() << "OutputFile" << OutputFile << endl;
    }
}

//检查目录
void MainWindow::on_pushButton_check_clicked()
{
    bool ok;

    if( gCurrent_cmd != START_CONVERSION)
    {
        total_number = 0;
        done_number = 0;
        QueueSetNull(&gQueue_Data);

        InputFile = ui->lineEdit_InputFile->text();
        OutputFile = ui->lineEdit_OutputFile->text();
        traverse_dir(InputFile);

        ui->label_filenum->setText("文件个数：" + QString::number(total_number, 10));
    }
    else
        QMessageBox::critical(NULL, tr("警告"), tr("转换时不能进行此操作!"), QMessageBox::Yes, QMessageBox::Yes);
}

//开始转换
void MainWindow::on_pushButton_start_clicked()
{
    if( gCurrent_cmd != START_CONVERSION)
    {
        gCurrent_cmd = START_CONVERSION;
    }
    else
        QMessageBox::critical(NULL, tr("警告"), tr("不能重复开始!"), QMessageBox::Yes, QMessageBox::Yes);

}

//结束转换
void MainWindow::on_pushButton_stop_clicked()
{
    if( gCurrent_cmd == START_CONVERSION)
    {
        gCurrent_cmd = CONVERSION_FINISH;
    }
    else
        QMessageBox::critical(NULL, tr("警告"), tr("无效的停止!"), QMessageBox::Yes, QMessageBox::Yes);
}

//更新状态显示
void MainWindow::slots_updata_state(unsigned int done_file, unsigned int total_file, Data_Node * data)
{
    char str[50];
    if( gCurrent_cmd == START_CONVERSION )
    {
        sprintf(str, "文件%d/%d 总进度%d%%", done_file, total_file, (done_file*100)/total_file);
        ui->progressBar->setFormat(str);
        ui->progressBar->setValue((done_file*100)/total_file);
        ui->label_CurrentInput->setText("正在转换：" + QString(QLatin1String(data->InputFilePath)));
        ui->label_CurrentOutput->setText("输出文件：" + QString(QLatin1String(data->OutputFilePath)));
    }
    else if( gCurrent_cmd == CONVERSION_FINISH )
    {
        gCurrent_cmd = NO_INSTRUCT;
        ui->progressBar->setFormat(tr("转换完成"));
    }
}

