#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mybutton.h"
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>

#include <QTime>
#include <QThread>
#include <QTimer>
#include <QPixmap>
#include <QSize>
#include <QProcess>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QListWidget>
#include <QStringList>
#include <QListWidgetItem>
#include <QByteArray>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getNameFromPath(const QString &fileName);
    QString removeIndexFromAppend( QString name);        //移除append序号
    void initDatabase(); //从路径中获取文件名
    void updateToDataBase();

private slots:
    void play();
//    void startSlot();
//    void pauseSlot();
    void playOrPauseSlot();
    void soundOrMuteSlot();
    void stopSlot();
    void nextSlot();
    void lastSlot();
    void addFileSlot();
    void deleteFileSlot();

    void on_playPosSlider_sliderPressed();

    void on_playPosSlider_sliderReleased();

    void on_playPosSlider_sliderMoved(int position);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_voiceSlider_sliderMoved(int position);

    void on_clearButton_clicked();

private:
    void Init();
    void updateDisplayPosInfo();

private:
    Ui::MainWindow *ui;

    QWidget*        buttonWidget;
    QMediaPlayer*   mediaPlayer;
    QVideoWidget*   videoWidget;
    QMediaPlaylist* mediaPlayList;
    PlayButton*     playButton;
    PauseButton*    pauseButton;
    StopButton*     stopButton;
    LastButton*     lastButton;
    NextButton*     nextButton;
    SoundButton*    soundButton;
    MuteButton*     muteButton;
    int playingIndex;           //当前正在播放视频的序号(从0开始)
    //用来保存文件目录 文件名
    QVector<QPair<QPair<QString, QString>, QString > > vector;   //name  fileName  nameAppendIndex
};
#endif // MAINWINDOW_H
