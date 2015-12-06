#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);

    mainWidget = new QWidget(this);
    rightWidget = new QWidget(mainWidget);
    leftWidget = new QWidget(mainWidget);

    gridLayout = new QGridLayout(mainWidget);
    imagePanel = new QGridLayout(rightWidget);
    mainPanel = new QGridLayout(leftWidget);


    ///BUTTONS
    addInButtons();

    /*QPixmap currentImage("c:\\Users\\SMNM\\Pictures\\Default\\pic.jpg") = ;
    mainImage = new QLabel();


    mainImage->setPixmap(currentImage);


    mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);*/


    ///PICTURES

    addImage("c:\\Users\\SMNM\\Pictures\\Default\\beach.jpg");
    noOfImages++;
    addImage("c:\\Users\\SMNM\\Pictures\\Default\\room.jpg");
    noOfImages++;
    addImage("c:\\Users\\SMNM\\Pictures\\Default\\fire.jpg");
    noOfImages++;

    rightWidget->setLayout(imagePanel);
    leftWidget->setLayout(mainPanel);

    gridLayout->addWidget(leftWidget,0,0);
    gridLayout->addWidget(rightWidget,0,1);

    mainWidget->setLayout(gridLayout);


    //imageView->setLayout(imagePanel);
    //gridLayout->addWidget(imageView,0,1);
    setCentralWidget(mainWidget);


    // CameraLink(std::string un, std::string p, int IPStart, int IPEnd, std::string port)
    camera = new andreasvh::CameraLink("admin", "1234", 100, 150, "8080");
    /*camera.setUsername("admin");
    camera.setPassword("1234");
    camera.setStartIP(100);
    camera.setEndIP(150);
    camera.setPort("8080");*/


    if (!camera->setUpConnection()){
        std::cout << "Error connection to camera" << std::endl;
    }
    else{
        std::cout << "Connection to camera successful" << std::endl;

    }


    cv::VideoCapture videocap;
    cv::Mat outimage;

    if(!videocap.open(camera->getConnectionAddress())) {
        std::cout << "Error opening video stream for " << std::endl;
    }






   // for(;;) {
         if(!videocap.read(outimage)) {
             std::cout << "No frame" << std::endl;
             cv::waitKey();
         }

         QPixmap currentImage = QPixmap::fromImage(QImage((unsigned char*) outimage.data, outimage.cols, outimage.rows, QImage::Format_RGB888).rgbSwapped());
         mainImage = new QLabel();


         mainImage->setPixmap(currentImage);
         mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);
         //cv::imshow("Output Window", image);
         //if(cv::waitKey(1) >= 0) break;
    // }

}




//QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888).rgbswapped());


void MainWindow::setUpLayout(){

}

void MainWindow::addInButtons(){
    QPushButton * liveFeed = new QPushButton("Live Feed");
    QPushButton * shootMode = new QPushButton("Shooting Mode");
    QPushButton * nextShot = new QPushButton("Show Next Shot");



    mainPanel->addWidget(liveFeed,0,0);
    mainPanel->addWidget(shootMode,0,1);
    mainPanel->addWidget(nextShot,2,0,1,2);
}

void MainWindow::addImage(std::string path){
    QPixmap image(path.c_str());
    QLabel * imageLabel = new QLabel();
    imageLabel->setPixmap(image);
    imagePanel->addWidget(imageLabel,noOfImages,0,Qt::AlignCenter);
    images.push_back(imageLabel);
}

void MainWindow::removeImage(std::string path){

    for (int i = 0; i < images.size(); i++){

    }
}

void MainWindow::setImageAmount(int size){
    noOfImages = size;
}


MainWindow::~MainWindow()
{
    delete ui;
    delete camera;
}

