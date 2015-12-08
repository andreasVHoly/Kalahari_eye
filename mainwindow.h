#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtWidgets>
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Cameralink.h"
#include <iostream>


namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setUpLayout();
    void addInButtons();
    void addImageFromDrive(std::string path);
    void addImageToPanel(QImage image);
    void removeImage(std::string path);
    void setImageAmount(int size);

private:
    Ui::MainWindow *ui;
    QWidget * mainWidget;
    QWidget * leftWidget;
    QWidget * rightWidget;
    QGridLayout * mainLayout;
    QGridLayout * imagePanel;
    QGridLayout * mainPanel;
    //std::vector<QLabel*> images;

    int noOfImages;
    andreasvh::CameraLink * camera;

    //qimages
    QImage * mainImageOriginal;
    QImage * mainImageEdited;
    std::vector<QLabel*> images;
    QLabel * mainImage;
    //mats
    cv::Mat matOriginal;
    cv::Mat matEdited;

    //video capture
    cv::VideoCapture videocap;


    //BUTTONS
    QPushButton * liveFeed ;
    QPushButton * shootMode;
    QPushButton * nextShot;


    //Timer
    QTimer * liveFeedTimer;

public slots:
    void updateFeed();

private slots:
    void on_LiveFeedBtnPress();
    void on_ShootingBtnPress();
    void on_ShowNextShotBtnPress();


};

#endif // MAINWINDOW_H

