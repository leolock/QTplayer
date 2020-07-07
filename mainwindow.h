#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mybutton.h"
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>

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

private slots:
//    void play();
    void startSlot();
    void pauseSlot();
    void addFileSlot();
    void deleteFileSlot();
    void setPosition(int position);
    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::MainWindow *ui;

    QWidget*        buttonWidget;
    QMediaPlayer*   mediaPlayer;
    QVideoWidget*   videoWidget;
    QMediaPlaylist* mediaPlayList;
    PlayButton*     playButton;
    PauseButton*    pauseButton;
    LastButton*     lastButton;
    NextButton*     nextButton;
    void Init();

//private slots:
//    void play();
//    void mediaStateChanged(QMediaPlayer::State state);
//    void positionChanged(qint64 position);
//    void setPosition(int position);
//    void durationChanged(qint64 duration);
//    void handleError();

//    void on_toolButton_clicked();

};
#endif // MAINWINDOW_H
