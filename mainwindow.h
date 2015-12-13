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
#include "clickablelabel.h"
#include <QKeyEvent>


namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //state enum
    enum SystemState {
        mode_shooting = 0,
        mode_live,
        mode_novid,
        mode_imageView
    };


    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setUpLayout();
    void addInButtons();
    void addImageFromDrive(std::string path);
    void addImageToPanel(QImage image);
    void removeImage(std::string path);
    void setImageAmount(int size);
    bool compareMatImages();
    bool compareQImages();
    void createMenuBar();
    void createActions();
    void connectToCamera();
    //key press handling
    void keyPressEvent(QKeyEvent * e);

private:
    Ui::MainWindow *ui;
    QWidget * mainWidget;
    QWidget * leftWidget;
    QWidget * rightWidget;
    QGridLayout * mainLayout;
    QGridLayout * imagePanel;
    QGridLayout * mainPanel;
    QScrollArea * scrollArea;
    //std::vector<QLabel*> images;

    int noOfImages;
    int threshold = 0;
    andreasvh::CameraLink * camera;

    //qimages
    QImage qImgOriginal;
    QImage qImgEdited;
    QImage qImgPrevious;
    std::vector<QLabel*> images;
    QLabel * mainImage;
    //mats
    cv::Mat matOriginal;
    cv::Mat matEdited;
    cv::Mat matPrevious;

    //video capture
    cv::VideoCapture videocap;


    //BUTTONS
    QPushButton * liveFeed ;
    QPushButton * shootMode;
    QPushButton * nextShot;


    //Timer
    QTimer * liveFeedTimer;

    //state
    SystemState currentState;

    //menu bar items
    QMenu * fileMenu;
    QMenu * modeMenu;
    QMenu * helpMenu;

    //menu bar actions
    QAction * exitAction;
    QAction * saveSessionAction;
    QAction * newSessionAction;
    QAction * shootingModeAction;
    QAction * liveVideoModeAction;
    QAction * aboutAction;
    QAction * tutorialLinkAction;
    QAction * contactAction;


    //scroll area
    QVBoxLayout * imageList;

    QColor * redColor;


public slots:
    void updateFeed();

private slots:
    void on_LiveFeedBtnPress();
    void on_ShootingBtnPress();
    void on_ShowNextShotBtnPress();
    void on_ImagePress();
    void exitApp();
    void saveSession();
    void newSession();
    void saveIdivImage();


};

#endif // MAINWINDOW_H

