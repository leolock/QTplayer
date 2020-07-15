#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mybutton.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    playingIndex(-1)
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
    videoWidget->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
    mediaPlayer->setVideoOutput(videoWidget);

//    建立播放列表
    mediaPlayList = new QMediaPlaylist;
    mediaPlayer->setPlaylist(mediaPlayList);
    mediaPlayer->setMedia(QUrl::fromLocalFile("/home/lsp/test.mp4"));

//    创建播放器按钮
//    buttonWidget = new QWidget(this);
    playButton = new PlayButton(ui->playWidget);
    pauseButton = new PauseButton(ui->playWidget);
    stopButton = new StopButton(ui->stopWidget);
    nextButton = new NextButton(ui->nextWidget);
    lastButton = new LastButton(ui->lastWidget);
    soundButton = new SoundButton(ui->soundWidget);
    muteButton = new MuteButton(ui->soundWidget);
    pauseButton->hide();
    muteButton->hide();
    ui->voiceSlider->setMaximum(100);
//    ui->widget->set
//    startButton = new MyButton(ui->widget);
//    this->setWindowFlags(Qt::FramelessWindowHint);
//    ui->buttonLayout->addWidget(buttonWidget);

//    鼠标停靠提示
    playButton->setToolTip("开始");
    stopButton->setToolTip("停止");
    soundButton->setToolTip("静音");
//    ui->startButton->setToolTip("暂停");
//    ui->startButton->setIcon(QPixmap(":/images/start.png"));
    ui->playPosSlider->setRange(0,mediaPlayer->duration()/1000);

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

    QObject::connect(playButton,SIGNAL(clicked()),this,SLOT(playOrPauseSlot()));//开始
    QObject::connect(pauseButton,SIGNAL(clicked()),this,SLOT(playOrPauseSlot()));//暂停
    QObject::connect(nextButton,SIGNAL(clicked()),this,SLOT(nextSlot()));//暂停
    QObject::connect(lastButton,SIGNAL(clicked()),this,SLOT(lastSlot()));//暂停
    QObject::connect(stopButton,SIGNAL(clicked()),this,SLOT(stopSlot()));//停止
    QObject::connect(soundButton,SIGNAL(clicked()),this,SLOT(soundOrMuteSlot()));//静音
    QObject::connect(muteButton,SIGNAL(clicked()),this,SLOT(soundOrMuteSlot()));//打开声音
    QObject::connect(ui->addfileButton,SIGNAL(clicked()),this,SLOT(addFileSlot()));//添加文件
    QObject::connect(ui->delfileButton,SIGNAL(clicked()),this,SLOT(deleteFileSlot()));//删除文件

//    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(slotPositionChanged(qint64)));

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
        ui->playPosSlider->setValue(position);
        updateDisplayPosInfo();
    });
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        ui->playPosSlider->setMaximum(duration);
        ui->playPosSlider->setValue(0);
    });

    initDatabase();

}
void MainWindow::play()
{
    videoWidget->show();
    mediaPlayer->play();

    playButton->hide();
    pauseButton->show();
    pauseButton->setToolTip("暂停");
}
void MainWindow::playOrPauseSlot()//暂停或者开始
{
    if(ui->listWidget->count() == 0)
        {
            QMessageBox::information(this,"open false","please add file first");
        }
    else
    {
        //    开始播放
        if(mediaPlayer->state() != QMediaPlayer::PlayingState)
        {
            play();
        }
        else
        {
            mediaPlayer->pause();
            pauseButton->hide();
            playButton->show();
            playButton->setToolTip("开始");
        }
    }
}
//停止
void MainWindow::stopSlot()
{
    mediaPlayer->stop();
    pauseButton->hide();
    playButton->show();
    ui->playPosSlider->setValue(0);
    ui->playPosSlider->setMaximum(mediaPlayer->duration());
    updateDisplayPosInfo();
}

//下一曲
void MainWindow::nextSlot()
{
    if(!vector.isEmpty())
    {
        if(++playingIndex == vector.size())
            playingIndex=0;

//        ui->listWidget->setIndex(playingIndex);
        mediaPlayer ->setMedia(QUrl::fromLocalFile(vector.at(playingIndex).first.second));
        play();
        ui->listWidget->setCurrentRow(playingIndex);
    }
}

//上一曲
void MainWindow::lastSlot()
{
    if(!vector.isEmpty())
    {
        //int currentIndex=playList->currentIndex();
        if(--playingIndex <= -1)
            playingIndex=vector.size()-1;
        //playList->setCurrentIndex(currentIndex);
//        listWidget->setIndex(playingIndex);
        mediaPlayer ->setMedia(QUrl::fromLocalFile(vector.at(playingIndex).first.second));
        play();
        ui->listWidget->setCurrentRow(playingIndex);
    }
}
void MainWindow::soundOrMuteSlot()//静音
{
    if(mediaPlayer->isMuted())
    {
        qDebug()<<"mute";
        mediaPlayer->setMuted(0);
        muteButton->hide();
        soundButton->show();
        muteButton->setToolTip("打开声音");
    }
    else
    {
        mediaPlayer->setMuted(1);
        soundButton->hide();
        muteButton->show();
        soundButton->setToolTip("静音");
    }

}
//过滤信号
QString MainWindow::removeIndexFromAppend(QString name)
{
    QString temp = name.right(2);
    temp=temp.left(1);
    if(temp=="0")
    {
        return (name.remove(name.size()-3, 3));
    }
    else
    {
        return name;
    }
}
void MainWindow::addFileSlot()//打开文件添加列表
{
    QString fileName = QFileDialog::getOpenFileName(this, "open file", QDir::currentPath());
    if(!fileName.isEmpty())
    {
        QString name = getNameFromPath(fileName);   //从路径中获取文件名字
        //titleLabel->setText(name);
        int i;
        for(i=0;i<vector.size();i++)
        {
            if(vector.at(i).first.second == fileName)   //该文件已经存在
            {
                playingIndex=i;
                mediaPlayer ->setMedia(QUrl::fromLocalFile(fileName));
                play();
                ui->listWidget->setCurrentRow(i);
//                ui->listWidget->setIndex(i);
//                ui->titleLabel->setText(removeIndexFromAppend(vector.at(i).second));
                break;
            }
        }
        if(i == vector.size())   // 相等说明不存在，则插入
        {
            //qDebug()<<"相等说明不存在，则插入";
            int j=0;
            QString theGoal;
            for(j=0; j<vector.size(); j++)
            {
                //存在不同路径但是有相同名字的name, 则查找nameAppendIndex中最大的序号
                if(name == vector.at(j).first.first)
                {
                      theGoal = vector.at(j).second;
                }
            }
            if(!theGoal.isEmpty())//theGoal非空， 说明有相同的name，此时要加上序号了
            {
                int n=(theGoal.right(2)).left(1).toInt();   //查找到该序号，即将插入的项序号需要比此序号大一
                QString nameAppend = name+"("+QString::number(n+1)+")";
                QPair<QString, QString>pairFirst(name, fileName);
                QPair<QPair<QString, QString>, QString> pair(pairFirst, nameAppend);
                vector.push_back(pair);

                QSqlTableModel mediaTableModel;
                mediaTableModel.setTable("mediatable");
                mediaTableModel.select();
                QSqlRecord record = mediaTableModel.record();

                QString index=QString::number(vector.size()-1);
                record.setValue(0,index);
                record.setValue(1,name);
                record.setValue(2, fileName);
                record.setValue(3, nameAppend);
                mediaTableModel.insertRecord(-1,record);
                mediaTableModel.submitAll();

                QListWidgetItem * item = new QListWidgetItem(nameAppend);
                qDebug()<<"nameAppend:  "<<nameAppend;
//                titleLabel->setText(nameAppend);
                ui->listWidget->addItem(item);

            }

            else //说明没有和name一样的，则直接插入
            {
                QPair<QString, QString>pairFirst(name, fileName);
                QString nameAppend = name+"(0)";
                QPair<QPair<QString, QString>, QString> pair(pairFirst, nameAppend);
                vector.push_back(pair);

                QSqlTableModel mediaTableModel;
                mediaTableModel.setTable("mediatable");
                mediaTableModel.select();

                QSqlRecord record = mediaTableModel.record();
                QString index=QString::number(vector.size()-1);
                 record.setValue(0,index);
                 record.setValue(1,name);
                 record.setValue(2, fileName);
                 record.setValue(3, nameAppend);
                 mediaTableModel.insertRecord(-1,record);
                 mediaTableModel.submitAll();

                QListWidgetItem * item = new QListWidgetItem(name);

//                titleLabel->setText(name);
                ui->listWidget->addItem(item);
            }

            if(!playButton->isEnabled())
            {
                playButton->setEnabled(true);
            }
            mediaPlayer ->setMedia(QUrl::fromLocalFile(fileName));
            play();
            playingIndex = vector.size()-1;
            ui->voiceSlider->setValue(mediaPlayer->volume()); //修改声音滑动条
            ui->listWidget->setCurrentRow(playingIndex);
//            ui->listWidget->setIndex(playingIndex);

        }
    }
//    int k=0;
//    QStringList filenames = QFileDialog::getOpenFileNames(this,"选择文件","/",tr("视频文件(*mp3 *mp4 *wma *3gp *wav *avi *rmvb *mkv *ts *wmv)"));
//    //筛选重复项
//    if(ui->listWidget->count() != -1)
//    {
//        for (QStringList::Iterator j=filenames.begin();j!=filenames.end();++j)
//        {
//            for(int i=0;i<ui->listWidget->count();++i)
//            {
//                if(*j == ui->listWidget->item(i)->text())
//                {
//                    k=1;
//                    continue;
//                }
//            }
//            if(k!=1)
//            {
//                ui->listWidget->addItem(*j);
//            }
//            else
//            {
//                k=0;
//            }
//        }

//    }
//    else
//    {
//        if(filenames.count()!=0)
//        {
//            ui->listWidget->addItems(filenames);
//        }
//    }

    //将listWidget中的项内容写入到txt文本中
//    QStringList lines;
//    QString line;
//    int row=0;
//    QFile file("x.txt");//打开该文件进入编辑模式
//    if(file.open(QIODevice::WriteOnly))//如果被打开
//    {
//        file.resize("x.txt",0);//清空内容
//        while(row<(ui->listWidget->count()))//从wistlidget里一行一行的读取文本并存入lines里边
//        {
//            line=ui->listWidget->item(row)->text();
//            row++;
//            lines<<line;
//        }
//        QTextStream stream( &file );//开始写入文本
//        for ( QStringList::Iterator it=lines.begin();it!=lines.end();++it )
//        stream<<*it<<"\r\n";//注意是"\r\n"！！！！！！如果是单个"\n"  保存文本的时候它不保留换行符！！！！
//    }
//    file.close();

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

//删除列表项
void MainWindow::deleteFileSlot()
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

//清空列表
void MainWindow::on_clearButton_clicked()
{
    mediaPlayer->stop();
    qDebug()<<"mediaPlayer0->stop() ";
    playingIndex = 0;
    ui->listWidget->clear();
    QSqlTableModel mediaTableModel;
    mediaTableModel.setTable("mediatable");
    mediaTableModel.select();

    for(int i=0;i<vector.size()+1;i++)
    {
        mediaTableModel.removeRow(i);
    }
    mediaTableModel.submitAll();
    vector.clear();
//    titleLabel->clear();
    qDebug()<<vector.isEmpty();
}

//从路径中得到文件名字
QString MainWindow::getNameFromPath(const QString &fileName)
{
    QString name;
    QChar c;
    QString s = fileName.toUtf8().data();
    foreach(c, s)
    {
        if(c =='/')
        {
            name.clear();
            continue;
        }
        name.append(c);
    }
    //name = name.toUtf8().data();     //解决name中文乱码问题
    return name;
}

//使用数据库的数据初始化
void MainWindow::initDatabase()
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("media.db");
//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
//    db.setHostName("localhost");
//    db.setDatabaseName("media");
//    db.setUserName("multin");
//    db.setPassword("drama");


    if (!db.open())
    {
        QMessageBox::critical(0, QObject::tr("Database Error"),db.lastError().text());
    }
    else
    {
        QSqlQuery query;
        //query.exec("create database media");
        if(!query.exec("CREATE TABLE mediatable(ind VARCHAR(777), name VARCHAR(777), fileName VARCHAR(777), nameAppend VARCHAR(777))"))
        {
            qDebug()<<query.lastError().text();
        }
        if(!query.exec("CREATE TABLE setting(volume VARCHAR(777), playMode VARCHAR(777))"))
        {
            qDebug()<<query.lastError().text();
        }

        QSqlTableModel settingModel;
        settingModel.setTable("setting");
        settingModel.select();
        QSqlRecord record=settingModel.record(0);

        ui->voiceSlider->setValue(record.value(0).toInt());
//        playModeListWidget->setCurrentRow(record.value(1).toInt());

        QSqlTableModel mediaTableModel;
        mediaTableModel.setTable("mediatable");
        mediaTableModel.select();
        int ret = mediaTableModel.rowCount();
        for(int i=0; i<ret;i++)
        {
            QSqlRecord record=mediaTableModel.record(i);
            QString name=record.value(1).toString();
            QString fileName = record.value(2).toString();
            QString nameAppend = record.value(3).toString();
            QPair<QString, QString> pair1(name, fileName);
            QPair<QPair<QString, QString>, QString>pair(pair1,nameAppend);
            vector.push_back(pair);
            //playList->addMedia(QUrl::fromLocalFile(fileName));
            ui->listWidget->addItem(removeIndexFromAppend(nameAppend));

            if(vector.isEmpty())
                playButton->setEnabled(false);
            else
                playButton->setEnabled(true);
        }
        //qDebug()<<"inidatabase finished";
    }

}

void MainWindow::on_playPosSlider_sliderPressed()
{
    mediaPlayer->pause();
}

void MainWindow::on_playPosSlider_sliderReleased()
{
    mediaPlayer->play();
}

void MainWindow::on_playPosSlider_sliderMoved(int position)
{
    mediaPlayer->setPosition(position);
}

void MainWindow::updateDisplayPosInfo()
{
    auto convertDispalyStr = [](int totalMilliseconds) -> QString {
        QTime tm = QTime(0, 0, 0, 0).addMSecs(totalMilliseconds);
        return  tm.toString("hh:mm:ss");
    };
    const QString position = convertDispalyStr(ui->playPosSlider->value());
    const QString duration = convertDispalyStr(ui->playPosSlider->maximum());
    ui->playPosLable->setText(QString("%1 / %2").arg(position).arg(duration));
}


void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString name = item->text();
    int i;
    QString temp = name.append("(0)");
    for(i=0;i<vector.size();i++)
    {
        qDebug()<<"vector:  "<<vector.at(i).second;
        if(vector.at(i).second == name)
        {
            //qDebug()<<"vector.at(i).second:  "<<vector.at(i).second;
            //qDebug()<<"if(vector.at(i).second == name)";
            ui->listWidget->setCurrentRow(i);
//            ui->listWidget->setIndex(i);  //为了使鼠标点击一下listWidget后，仍然不改变当前item
//            titleLabel->setText(removeIndexFromAppend(vector.at(i).second));
            playingIndex = i;
            qDebug()<<"vector.at(i).first.second:  "<<vector.at(i).first.second;
            mediaPlayer ->setMedia(QUrl::fromLocalFile(vector.at(i).first.second));
            play();
            break;
        }

        if(vector.at(i).second ==temp )   //该文件已经存在
        {
            ui->listWidget->setCurrentRow(i);
//            ui->listWidget->setIndex(i);  //为了使鼠标点击一下listWidget后，仍然不改变当前item
//            titleLabel->setText(removeIndexFromAppend(vector.at(i).second));
            playingIndex = i;
            qDebug()<<"vector.at(i).first.second:  "<<vector.at(i).first.second;
            mediaPlayer ->setMedia(QUrl::fromLocalFile(vector.at(i).first.second));
            play();
            break;
        }
    }
    //qDebug()<<"slotListWidgetItemDoubleClick finished";
//    if (item->
//            !item.isValid())
//    {
//        return ;
//    }
//    QString name = ui->playList->item(index.row())->text();
//    if (!m_ulrMap.contains(name))
//    {
//        ui->statusbar->showMessage("播放失败，没有对应的ulr地址");
//        if (QMessageBox::warning(this, "警告", "当前影视不能播放，是否删除", "是", "否") == 0)
//        {
//            m_nameList.removeAll(name);
//            m_ulrMap.remove(name);
//            ui->playList->takeItem(index.row());
//            savePlayList();
//        }
//        return ;
//    }
//    mediaPlayList->setCurrentIndex(item.);
//    mediaPlayer->setMedia(QUrl::fromLocalFile(item->text()));
//    play();
//    QTimer::singleShot(800, this, [this](){
//        mediaPlayer->play();
//    });
}

void MainWindow::on_voiceSlider_sliderMoved(int position)
{
    if(position == 0)
    {
        mediaPlayer->setMuted(1);
        soundButton->hide();
        muteButton->show();
        soundButton->setToolTip("打开声音");
    }
    else
    {
        mediaPlayer->setVolume(position);
        mediaPlayer->setMuted(0);
        muteButton->hide();
        soundButton->show();
        muteButton->setToolTip("静音");
    }
}

//使用vector更新database
void MainWindow::updateToDataBase()
{
    QSqlTableModel mediaTableModel;
    mediaTableModel.setTable("mediatable");
    mediaTableModel.select();

    for(int i=0;i<vector.size()+1;i++)
    {
        mediaTableModel.removeRow(i);
    }

    QSqlRecord record = mediaTableModel.record();

    for(int i=0;i<vector.size();i++)
    {
        record.setValue(0,QString::number(i));
        record.setValue(1,vector.at(i).first.first);
        record.setValue(2, vector.at(i).first.second);
        record.setValue(3, vector.at(i).second);
        mediaTableModel.insertRecord(i,record);
    }
    mediaTableModel.submitAll();
}

