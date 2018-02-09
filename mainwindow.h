#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_open_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

    void slots_updata_state(unsigned int done_file, unsigned int total_file, Data_Node * data);

    void on_pushButton_check_clicked();

signals:
    void SendGUIThreadID(int);

private:
    Ui::MainWindow *ui;
    textTread *pWriteFile;       //写文件线程
};

#endif // MAINWINDOW_H
