#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mybutton.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Init();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
//    建立视频播放区域
    videoWidget = new QVideoWidget(this);
    ui->playWidgetLayout->addWidget(videoWidget);
//    指定视频输出位置
    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setVideoOutput(videoWidget);
//    建立播放列表
    mediaPlayList = new QMediaPlaylist(this);
    mediaPlayer->setPlaylist(mediaPlayList);
//    创建播放器按钮
//    buttonWidget = new QWidget(this);
    MyButton *startButton = new MyButton(ui->widget);
//    startButton = new MyButton(ui->widget);
//    this->setWindowFlags(Qt::FramelessWindowHint);
//    ui->buttonLayout->addWidget(buttonWidget);

//    设置按钮初始状态
    startButton->setToolTip("暂停");
    ui->startButton->setToolTip("暂停");
    ui->startButton->setIcon(QPixmap(":/images/start.png"));
//    QObject::connect(ui->startButton,SIGNAL(clicked()),this,SLOT(startorpauseSlot()));//开始暂停
    QObject::connect(startButton,SIGNAL(clicked()),this,SLOT(startorpauseSlot()));//开始暂停
}
void MainWindow::startorpauseSlot()//暂停或者开始
{
//    this->process->write("pause\n");
    if(ui->startButton->toolTip() == "暂停")
    {
        if(ui->listWidget->count() == 0)
        {
            QMessageBox::information(this,"open false","please add file first");
        }
        else
        {
//            QObject::connect(this->process,SIGNAL(readyReadStandardOutput()),this,SLOT(dataReceive()));
//            this->process->write("get_time_length\n");
//            this->process->write("get_time_pos\n");
//            this->process->write("get_percent_pos\n");
            mediaPlayer->play();

            QPixmap pixmap;
            pixmap.load("/home/lsp/Player/QTplayer/images/voice.png");
            QPixmap pixmap4;
            pixmap4.load("/home/lsp/Player/QTplayer/images/stop.png");
            startButton->set
            ui->startButton->setFixedSize(pixmap.width(),pixmap.height());
            ui->startButton->setIcon(pixmap4);
            ui->startButton->setIconSize(QSize(pixmap.width(),pixmap.height()));
            ui->startButton->setToolTip("开始");
            ui->startButton->show();

        }
    }
    else
    {
//        QObject::disconnect(this->process,SIGNAL(readyReadStandardOutput()),this,SLOT(dataReceive()));
        mediaPlayer->pause();
        QPixmap pixmap;
        pixmap.load("/home/lsp/Player/QTplayer/images/voice.png");
        QPixmap pixmap3;
        pixmap3.load("/home/lsp/Player/QTplayer/images/start.png");
        ui->startButton->setFixedSize(pixmap.width(),pixmap.height());
        ui->startButton->setIcon(pixmap3);
        ui->startButton->setIconSize(QSize(pixmap.width(),pixmap.height()));
        ui->startButton->setToolTip("开始");
        ui->startButton->show();

    }

}

void MainWindow::setPosition(int position)
{
    mediaPlayer->setPosition(position);
}
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    setPosition(position);
}
