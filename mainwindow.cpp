#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);

    mainWidget = new QWidget(this);
    rightWidget = new QWidget(mainWidget);
    leftWidget = new QWidget(mainWidget);
    mainLayout = new QGridLayout(mainWidget);
    imagePanel = new QGridLayout(rightWidget);
    mainPanel = new QGridLayout(leftWidget);
    mainImage = new QLabel();

    ///BUTTONS
    addInButtons();

    /*QPixmap currentImage("c:\\Users\\SMNM\\Pictures\\Default\\pic.jpg") = ;



    mainImage->setPixmap(currentImage);


    mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);*/


    ///PICTURES

    //addImage("c:\\Users\\SMNM\\Pictures\\Default\\beach.jpg");
   // addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\room.jpg");
   // addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\fire.jpg");


    rightWidget->setLayout(imagePanel);
    leftWidget->setLayout(mainPanel);

    mainLayout->addWidget(leftWidget,0,0);
    mainLayout->addWidget(rightWidget,0,1);

    mainWidget->setLayout(mainLayout);


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

    //std::cout << "test 1 passed" << std::endl;



    if(!videocap.open(camera->getConnectionAddress())) {
        std::cout << "Error opening video stream for " << std::endl;
        //TODO need to display this on the screen that there is no video feed & show default black screen
        throw EXCEPTION_ILLEGAL_INSTRUCTION;
    }

    liveFeedTimer = new QTimer(this);
    connect(liveFeedTimer, SIGNAL(timeout()), this, SLOT(updateFeed()));
    liveFeedTimer->start(20);
    //std::cout << "test 2 passed" << std::endl;


   // for(;;) {
       /*  if(!videocap.read(outimage)) {
             std::cout << "No frame" << std::endl;
             cv::waitKey();
         }

         QPixmap currentImage = QPixmap::fromImage(QImage((unsigned char*) outimage.data, outimage.cols, outimage.rows, QImage::Format_RGB888).rgbSwapped());
         mainImage = new QLabel();


         mainImage->setPixmap(currentImage);

         //cv::imshow("Output Window", image);
         //if(cv::waitKey(1) >= 0) break;
    // }*/

}




//QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888).rgbswapped());




void MainWindow::addInButtons(){
    liveFeed = new QPushButton("Live Feed");
    shootMode = new QPushButton("Shooting Mode");
    nextShot = new QPushButton("Show Next Shot");

    mainPanel->addWidget(liveFeed,0,0);
    mainPanel->addWidget(shootMode,0,1);
    mainPanel->addWidget(nextShot,2,0,1,2);
}


void MainWindow::addImageToPanel(QImage image){
    noOfImages++;
    QLabel * imageLabel = new QLabel();
    imageLabel->setPixmap(QPixmap::fromImage(image));
    imagePanel->addWidget(imageLabel,noOfImages,0,Qt::AlignCenter);
    images.push_back(imageLabel);
}


void MainWindow::addImageFromDrive(std::string path){
    noOfImages++;
    QPixmap image(path.c_str());
    QLabel * imageLabel = new QLabel();
    imageLabel->setPixmap(image);
    imagePanel->addWidget(imageLabel,noOfImages,0,Qt::AlignCenter);
    images.push_back(imageLabel);
}


void MainWindow::updateFeed(){
    //update the main feed
    //std::cout << "test 3 passed" << std::endl;
    //simple video update
    videocap.read(matOriginal);
    //if nothing is coming through
    if (matOriginal.empty()){
        return;
    }
   // std::cout << "test 4 passed" << std::endl;

    QPixmap currentImage = QPixmap::fromImage(QImage((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888).rgbSwapped());
   // std::cout << "test 5 passed" << std::endl;
    mainImage->setPixmap(currentImage);
    //std::cout << "test 6 passed" << std::endl;
    mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);
    //std::cout << "test 7 passed" << std::endl;
}


void MainWindow::on_ShowNextShotBtnPress(){
//handle show next button being pressed
}

void MainWindow::on_ShootingBtnPress(){
//handle shooting button being pressed
}

void MainWindow::on_LiveFeedBtnPress(){
//handle live feed button being pressed
}



void MainWindow::removeImage(std::string path){
/*
    for (int i = 0; i < images.size(); i++){

    }*/
}

void MainWindow::setImageAmount(int size){
    noOfImages = size;
}


MainWindow::~MainWindow()
{
    delete ui;
    delete camera;
}

