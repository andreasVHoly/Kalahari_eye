#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);

    images.clear();
   // images.reserve(3);
    currentState = SystemState::mode_novid;
    noOfImages = 0;
    //widgets
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
    camera = new andreasvh::CameraLink("admin", "1234", 101, 150, "8080");
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
        currentState = SystemState::mode_live;

    }

    //std::cout << "test 1 passed" << std::endl;



    if(!videocap.open(camera->getConnectionAddress())) {
        std::cout << "Error opening video stream for " << std::endl;
        //TODO need to display this on the screen that there is no video feed & show default black screen
        currentState = SystemState::mode_novid;
        throw EXCEPTION_ILLEGAL_INSTRUCTION;
    }

    liveFeedTimer = new QTimer(this);
    connect(liveFeedTimer, SIGNAL(timeout()), this, SLOT(updateFeed()));
    liveFeedTimer->start(20);
    //std::cout << "test 2 passed" << std::endl;




}




void MainWindow::addInButtons(){

    //assign clicking and pressing actions to nessesary slots
    liveFeed = new QPushButton("Live Feed");
    connect(liveFeed, SIGNAL(clicked()), this, SLOT(on_LiveFeedBtnPress()));
    connect(liveFeed, SIGNAL(pressed()), this, SLOT(on_LiveFeedBtnPress()));
    shootMode = new QPushButton("Shooting Mode");
    connect(shootMode, SIGNAL(clicked()), this, SLOT(on_ShootingBtnPress()));
    connect(shootMode, SIGNAL(pressed()), this, SLOT(on_ShootingBtnPress()));
    nextShot = new QPushButton("Show Next Shot");
    connect(nextShot, SIGNAL(clicked()), this, SLOT(on_ShowNextShotBtnPress()));
    connect(nextShot, SIGNAL(pressed()), this, SLOT(on_ShowNextShotBtnPress()));


    mainPanel->addWidget(liveFeed,0,0);
    mainPanel->addWidget(shootMode,0,1);
    mainPanel->addWidget(nextShot,2,0,1,2);
}


void MainWindow::addImageToPanel(QImage image){
   // std::cout << "start of method" << std::endl;
    noOfImages++;

    QLabel * imageLabel = new QLabel();
    imageLabel->setPixmap(QPixmap::fromImage(image.rgbSwapped()));
    imagePanel->addWidget(imageLabel,noOfImages,0,Qt::AlignCenter);

    if (images.empty()){
        images.reserve(noOfImages);
    }
    images.push_back(imageLabel);
   // std::cout << "end of method" << std::endl;
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
    //simple video update
    videocap.read(matOriginal);
    //if nothing is coming through
    if (matOriginal.empty()){
        return;
    }


    qImgOriginal = QImage((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);

    mainImage->setPixmap(QPixmap::fromImage(qImgOriginal.rgbSwapped()));

    mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);

}


void MainWindow::on_ShowNextShotBtnPress(){
//handle show next button being pressed
}

void MainWindow::on_ShootingBtnPress(){
//handle shooting button being pressed
    //already in shooting mode
    if (currentState == 0){
        return;
    }
    currentState = SystemState::mode_shooting;
    liveFeedTimer->stop();
    //std::cout << "shooting mode button pressed" << std::endl;
    addImageToPanel(qImgOriginal);
    //std::cout << "shooting mode button pressed2" << std::endl;
}

void MainWindow::on_LiveFeedBtnPress(){
//handle live feed button being pressed
    if (currentState == 1){
        return;
    }
    currentState = SystemState::mode_live;
    //start the timer again
    liveFeedTimer->start(20);
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

