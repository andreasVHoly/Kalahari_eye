#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
     std::cout << "starting" << std::endl;

    images.clear();
   // images.reserve(3);
    currentState = SystemState::mode_novid;
    noOfImages = 0;
    //widgets
    mainWidget = new QWidget(this);


    //RIGHT WIDGET
    //rightWidget = new QWidget(mainWidget);
    //rightWidget->setStyleSheet("*{background-color:rgb(50,50,50);}");


    //QAbstractScrollArea::setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //QAbstractScrollArea::setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea = new QScrollArea(mainWidget);
    scrollArea->setStyleSheet("*{background-color:rgb(50,50,50);}");

    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);







    //LEFT WIDGET
    leftWidget = new QWidget(mainWidget);
    leftWidget->setStyleSheet("*{background-color:rgb(100,50,50);}");
    //set the size of both widgets
     std::cout << "mid" << std::endl;
    int height = mainWidget->size().height();
    int width = mainWidget->size().width();
    std::cout << "mi2d" << std::endl;
    //rightWidget->setFixedWidth(width/4);
    //leftWidget->setFixedWidth(3*(width/4));

    std::cout << "mid" << std::endl;
    mainLayout = new QGridLayout(mainWidget);
    //imagePanel = new QGridLayout(rightWidget);
    mainPanel = new QGridLayout(leftWidget);
    mainImage = new QLabel();

    ///BUTTONS
    addInButtons();



    ///PICTURES

    addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\beach.jpg");
    addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\pic.jpg");
    addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\room.jpg");
    addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\fire.jpg");


    //rightWidget->setLayout(imagePanel);
    leftWidget->setLayout(mainPanel);

    mainLayout->addWidget(leftWidget,0,0);
    mainLayout->addWidget(scrollArea,0,1);

    mainWidget->setLayout(mainLayout);



    setCentralWidget(mainWidget);


    /*camera = new andreasvh::CameraLink("admin", "1234", 101, 150, "8080");



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
*/
     std::cout << "end of method" << std::endl;


}




void MainWindow::addInButtons(){

    //assign clicking and pressing actions to nessesary slots
    liveFeed = new QPushButton("Live Feed");
    connect(liveFeed, SIGNAL(clicked()), this, SLOT(on_LiveFeedBtnPress()));
    //connect(liveFeed, SIGNAL(pressed()), this, SLOT(on_LiveFeedBtnPress()));
    shootMode = new QPushButton("Shooting Mode");
    connect(shootMode, SIGNAL(clicked()), this, SLOT(on_ShootingBtnPress()));
    //connect(shootMode, SIGNAL(pressed()), this, SLOT(on_ShootingBtnPress()));
    nextShot = new QPushButton("Show Next Shot");
    connect(nextShot, SIGNAL(clicked()), this, SLOT(on_ShowNextShotBtnPress()));
    //connect(nextShot, SIGNAL(pressed()), this, SLOT(on_ShowNextShotBtnPress()));


    mainPanel->addWidget(liveFeed,0,0);
    mainPanel->addWidget(shootMode,0,1);
    mainPanel->addWidget(nextShot,2,0,1,2);
}


void MainWindow::addImageToPanel(QImage image){
   // std::cout << "start of method" << std::endl;
    noOfImages++;

    QLabel * imageLabel = new QLabel();
    imageLabel->setPixmap(QPixmap::fromImage(image.rgbSwapped()));
    //imagePanel->addWidget(imageLabel,noOfImages,0,Qt::AlignCenter);
    scrollArea->addScrollBarWidget(imageLabel,Qt::AlignCenter);
    if (images.empty()){
        images.reserve(noOfImages);
    }
    images.push_back(imageLabel);
   // std::cout << "end of method" << std::endl;
}


void MainWindow::addImageFromDrive(std::string path){
    noOfImages++;
    QPixmap image(path.c_str());
    ClickableLabel * imageLabel = new ClickableLabel(image, this);
    //imageLabel->setPixmap(image);

    connect(imageLabel, SIGNAL(clicked()), this, SLOT(on_ImagePress()));
    //connect(imageLabel, SIGNAL(pressed()), this, SLOT(on_ImagePress(image)));

    //imagePanel->addWidget(imageLabel,noOfImages,0,Qt::AlignCenter);
    scrollArea->addScrollBarWidget(imageLabel,Qt::AlignCenter);
    if (images.empty()){
        images.reserve(noOfImages);
    }
    images.push_back(imageLabel);
}


void MainWindow::updateFeed(){
    //update the main feed
    //simple video update
    videocap.read(matOriginal);


    if (currentState != 1){
        return;
    }

    //if nothing is coming through
    if (matOriginal.empty()){
        return;
    }


    qImgOriginal = QImage((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);

    mainImage->setPixmap(QPixmap::fromImage(qImgOriginal.rgbSwapped()));

    mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);

}

bool MainWindow::compareImages(){
//compare pics
    std::cout << "compare image method" << std::endl;
    std::cout <<  matOriginal.rows << "," << matOriginal.cols <<std::endl;
    std::cout <<  matPrevious.rows << "," << matPrevious.cols <<std::endl;
    if (matOriginal.rows != matPrevious.rows || matOriginal.cols != matPrevious.cols){
        std::cout << "image sizes do not match" << std::endl;
        return false;
    }
    matEdited = matOriginal;

    if (matEdited.empty()){
        return false;
    }

    std::cout << "compare image prelims passed" << std::endl;

    for (int i = 0; i < matPrevious.rows; i++){
        for (int j = 0; j < matPrevious.cols; j++){

            int diff = std::abs((int)matPrevious.at<uchar>(i,j) - (int)matOriginal.at<uchar>(i,j));

            if ((int)matPrevious.at<uchar>(i,j) != (int)matOriginal.at<uchar>(i,j)){
                std::cout << (int)matPrevious.at<uchar>(i,j) << std::endl;
                std::cout << (int)matOriginal.at<uchar>(i,j) << std::endl;
            }

            if (diff > threshold){
                std::cout << "found pixel" << std::endl;
                matEdited.at<cv::Vec3b>(i,j)[0] = 0;
                matEdited.at<cv::Vec3b>(i,j)[1] = 0;
                matEdited.at<cv::Vec3b>(i,j)[2] = 255;
            }

            if (diff > 0){
                std::cout << diff << std::endl;
            }

            //std::cout << (int)image.at<cv::Vec3b>(i,j)[0] << " " << (int)image.at<cv::Vec3b>(i,j)[1] << " " << (int)image.at<cv::Vec3b>(i,j)[2] << ",";
        }



    }

    return true;


}

void MainWindow::keyPressEvent(QKeyEvent * e){
    std::cout << "event fire" << std::endl;

    if (e->key() == Qt::Key_Space){
        std::cout << "space pressed" << std::endl;
    }


   /* if (e->text() == "Space"){
        std::cout << "space pressed" << std::endl;
    }*/
}


void MainWindow::on_ShowNextShotBtnPress(){
    if (currentState != 0){
        return;
    }


//handle show next button being pressed
    //first handle keyboard events (optional access), needs to be done in the update loop, otherwise this will be called by pressing the button
    std::cout << "next shot slot" << std::endl;
    //we back up the old image
    matPrevious = matOriginal;
    if (matPrevious.empty()){
        return;
    }

    //we read in a new image
    videocap.read(matOriginal);
    //if nothing is coming through
    if (matOriginal.empty()){
        return;
    }

    if (compareImages()){
        qImgEdited = QImage((uchar*)matEdited.data, matEdited.cols, matEdited.rows, matEdited.step, QImage::Format_RGB888);

        mainImage->setPixmap(QPixmap::fromImage(qImgEdited.rgbSwapped()));

        mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);
    }
    else{
        std::cout << "image compare failed" << std::endl;
    }




}




void MainWindow::on_ShootingBtnPress(){
//handle shooting button being pressed
    //already in shooting mode
    if (currentState == 0){
        return;
    }
    currentState = SystemState::mode_shooting;
    //liveFeedTimer->stop();
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
    //liveFeedTimer->start(20);
}

 void MainWindow::on_ImagePress(){
   // std::cout << QObject::sender()->isWidgetType() << std::endl;
    if (QObject::sender()->isWidgetType()){
        const QPixmap * map = qobject_cast<ClickableLabel*>(QObject::sender())->pixmap();
        QPixmap &temp = const_cast<QPixmap&>(*map);
        mainImage->setPixmap(temp);
        mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);
    }
    //std::cout << "image pressed" << std::endl;
    currentState = SystemState::mode_imageView;
   // (((ClickableLabel)(QObject::sender())).pixmap());


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

