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
    videoWidget = new QVideoWidget;
    ui->playWidgetLayout->addWidget(videoWidget);
//    指定视频输出位置
    mediaPlayer = new QMediaPlayer;
    mediaPlayer->setVideoOutput(videoWidget);
//    QFile file("/home/lsp/test.mp4");

//    建立播放列表
    mediaPlayList = new QMediaPlaylist;
    mediaPlayer->setPlaylist(mediaPlayList);
//    创建播放器按钮
//    buttonWidget = new QWidget(this);
    playButton = new PlayButton(ui->playWidget);
    pauseButton = new PauseButton(ui->playWidget);
    nextButton = new NextButton(ui->nextWidget);
    lastButton = new LastButton(ui->lastWidget);
    pauseButton->hide();
//    ui->widget->set
//    startButton = new MyButton(ui->widget);
//    this->setWindowFlags(Qt::FramelessWindowHint);
//    ui->buttonLayout->addWidget(buttonWidget);

//    鼠标停靠提示
    playButton->setToolTip("开始");
//    ui->startButton->setToolTip("暂停");
//    ui->startButton->setIcon(QPixmap(":/images/start.png"));

    QFile dataFile("x.txt");
    if (dataFile.open(QFile::ReadOnly|QIODevice::Text))
    {
        QTextStream data(&dataFile);
        QStringList fonts;
        QString line;
        while (!data.atEnd())//逐行读取文本，并去除每行的回车
        {
            line = data.readLine();
            line.remove('\n');
            fonts<<line;
        }
        ui->listWidget->addItems(fonts);//把各行添加到listwidget
    }

    QObject::connect(playButton,SIGNAL(clicked()),this,SLOT(startSlot()));//开始
    QObject::connect(pauseButton,SIGNAL(clicked()),this,SLOT(pauseSlot()));//暂停
    QObject::connect(ui->addfileButton,SIGNAL(clicked()),this,SLOT(addFileSlot()));//添加文件
    QObject::connect(ui->delfileButton,SIGNAL(clicked()),this,SLOT(deleteFileSlot()));//删除文件
}
void MainWindow::startSlot()//暂停或者开始
{
    qDebug()<<"slotPlayOrPause";
//    if(playButton->toolTip() == "开始")
//    {
        qDebug()<<"in";
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
            mediaPlayer->setMedia(QUrl::fromLocalFile("/home/lsp/test.mp4"));

            videoWidget->show();
            mediaPlayer->play();

            playButton->hide();
            pauseButton->show();
            pauseButton->setToolTip("暂停");

//            playButton = pauseButton;

//            QPixmap pixmap;
//            pixmap.load("/home/lsp/Player/QTplayer/images/voice.png");
//            QPixmap pixmap4;
//            pixmap4.load("/home/lsp/Player/QTplayer/images/stop.png");
////            startButton->set
//            playButton->setFixedSize(pixmap.width(),pixmap.height());
//            playButton->setIcon(pixmap4);
//            playButton->setIconSize(QSize(pixmap.width(),pixmap.height()));
//            playButton->setToolTip("暂停");
//            playButton->show();

//        }
    }
//    else
//    {
////        QObject::disconnect(this->process,SIGNAL(readyReadStandardOutput()),this,SLOT(dataReceive()));
//        mediaPlayer->pause();
//        QPixmap pixmap;
//        pixmap.load("/home/lsp/Player/QTplayer/images/voice.png");
//        QPixmap pixmap3;
//        pixmap3.load("/home/lsp/Player/QTplayer/images/start.png");
//        playButton->setFixedSize(pixmap.width(),pixmap.height());
//        playButton->setIcon(pixmap3);
//        playButton->setIconSize(QSize(pixmap.width(),pixmap.height()));
//        playButton->setToolTip("开始");
//        playButton->show();

//    }

}
void MainWindow::pauseSlot()//暂停
{
    qDebug()<<"slotPlayOrPause";
    mediaPlayer->pause();
    pauseButton->hide();
    playButton->show();
    playButton->setToolTip("开始");
}
void MainWindow::addFileSlot()//打开文件添加列表
{
    int k=0;
    QStringList filenames = QFileDialog::getOpenFileNames(this,"选择文件","/",tr("视频文件(*mp3 *mp4 *wma *3gp *wav *avi *rmvb *mkv *ts *wmv)"));
    //筛选重复项
    if(ui->listWidget->count() != -1)
    {

        for (QStringList::Iterator j=filenames.begin();j!=filenames.end();++j)
        {
            for(int i=0;i<ui->listWidget->count();++i)
            {
                if(*j == ui->listWidget->item(i)->text())
                {
                    k=1;
                    continue;
                }


            }
            if(k!=1)
            {
                ui->listWidget->addItem(*j);
            }
            else
            {
                k=0;
            }
        }

    }
    else
    {
        if(filenames.count()!=0)
        {
            ui->listWidget->addItems(filenames);
        }
    }

    //将listWidget中的项内容写入到txt文本中
    QStringList lines;
    QString line;
    int row=0;
    QFile file("x.txt");//打开该文件进入编辑模式
    if(file.open(QIODevice::WriteOnly))//如果被打开
    {
        file.resize("x.txt",0);//清空内容
        while(row<(ui->listWidget->count()))//从wistlidget里一行一行的读取文本并存入lines里边
        {
            line=ui->listWidget->item(row)->text();
            row++;
            lines<<line;
        }
        QTextStream stream( &file );//开始写入文本
        for ( QStringList::Iterator it=lines.begin();it!=lines.end();++it )
        stream<<*it<<"\r\n";//注意是"\r\n"！！！！！！如果是单个"\n"  保存文本的时候它不保留换行符！！！！
    }
    file.close();

}

//双击列表项更换视频
//void MainWindow::slotListWidgetItemDoubleClick(QString name)
//{
//    int i;
//    QString temp = name.append("(0)");
//    for(i=0;i<vector.size();i++)
//    {
//        qDebug()<<"vector:  "<<vector.at(i).second;
//        if(vector.at(i).second == name)
//        {
//            //qDebug()<<"vector.at(i).second:  "<<vector.at(i).second;
//            //qDebug()<<"if(vector.at(i).second == name)";
//            listWidget->setCurrentRow(i);
//            listWidget->setIndex(i);  //为了使鼠标点击一下listWidget后，仍然不改变当前item
//            titleLabel->setText(removeIndexFromAppend(vector.at(i).second));
//            playingIndex = i;
//            qDebug()<<"vector.at(i).first.second:  "<<vector.at(i).first.second;
//            mediaPlayer0 ->setMedia(QUrl::fromLocalFile(vector.at(i).first.second));
//            mediaPlayer0->play();
//            break;
//        }

//        if(vector.at(i).second ==temp )   //该文件已经存在
//        {
//            listWidget->setCurrentRow(i);
//            listWidget->setIndex(i);  //为了使鼠标点击一下listWidget后，仍然不改变当前item
//            titleLabel->setText(removeIndexFromAppend(vector.at(i).second));
//            playingIndex = i;
//            qDebug()<<"vector.at(i).first.second:  "<<vector.at(i).first.second;
//            mediaPlayer0 ->setMedia(QUrl::fromLocalFile(vector.at(i).first.second));
//            mediaPlayer0->play();
//            break;
//        }
//    }
//    //qDebug()<<"slotListWidgetItemDoubleClick finished";
//}
void MainWindow::deleteFileSlot()//删除
{
    if(ui->listWidget->currentRow() == -1)
    {
        QMessageBox::information(this,"Error delete","Not select a flie or thie list is null");
    }
    else
    {
        ui->listWidget->takeItem(ui->listWidget->currentRow());

        //将listWidget中的项内容写入到txt文本中
        QStringList lines;
        QString line;
        int row=0;
        QFile file("x.txt");//打开该文件进入编辑模式
        if(file.open(QIODevice::WriteOnly))//如果被打开
        {
            file.resize("x.txt",0);//清空内容
            while(row<(ui->listWidget->count()))//从wistlidget里一行一行的读取文本并存入lines里边
            {
                line=ui->listWidget->item(row)->text();
                row++;
                lines<<line;
            }
            QTextStream stream( &file );//开始写入文本
            for ( QStringList::Iterator it=lines.begin();it!=lines.end();++it )
            stream<<*it<<"\r\n";//注意是"\r\n"！！！！！！如果是单个"\n"  保存文本的时候它不保留换行符！！！！
        }
        file.close();
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
