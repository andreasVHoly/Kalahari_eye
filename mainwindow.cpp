#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    std::cout << "starting" << std::endl;

    redColor = new QColor(255,0,0);



    images.clear();
   // images.reserve(3);
    currentState = SystemState::mode_novid;
    noOfImages = 0;
    //widgets
    mainWidget = new QWidget(this);


    //RIGHT WIDGET
    rightWidget = new QWidget(mainWidget);
    rightWidget->setStyleSheet("*{background-color:rgb(50,50,50);}");


    scrollArea = new QScrollArea();
    imageList = new QVBoxLayout(scrollArea);
    scrollArea->setStyleSheet("*{background-color:rgb(100,100,50);}");

    scrollArea->setWidget(imageList->widget());
    scrollArea->setWidgetResizable(true);


    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setAlignment(Qt::AlignCenter);







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
    imagePanel = new QGridLayout(rightWidget);
    mainPanel = new QGridLayout(leftWidget);
    mainImage = new QLabel();

    ///BUTTONS
    addInButtons();



    ///PICTURES

    /*addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\beach.jpg");
    addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\pic.jpg");
    addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\room.jpg");
    addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\fire.jpg");
    addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\fire.jpg");
    addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\fire.jpg");
    addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\fire.jpg");
    addImageFromDrive("c:\\Users\\SMNM\\Pictures\\Default\\fire.jpg");*/

    imagePanel->addWidget(scrollArea);
    rightWidget->setLayout(imagePanel);
    leftWidget->setLayout(mainPanel);

    mainLayout->addWidget(leftWidget,0,0);
    mainLayout->addWidget(rightWidget,0,1);

    mainWidget->setLayout(mainLayout);


    setCentralWidget(mainWidget);
    createActions();
    createMenuBar();


    bool debugCamStart = true;
    if (debugCamStart){
        connectToCamera();
    }


     std::cout << "end of method" << std::endl;


}


void MainWindow::connectToCamera(){
    camera = new andreasvh::CameraLink("admin", "1234", 101, 150, "8080");

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

    textBox = new QLineEdit();
    textBox->setText(QString::number(threshold));


    mainPanel->addWidget(liveFeed,0,0);
    mainPanel->addWidget(shootMode,0,1);
    //mainPanel->addWidget(nextShot,2,0,1,2);
    mainPanel->addWidget(nextShot,2,1);
    mainPanel->addWidget(textBox,2,0);
}


void MainWindow::addImageToPanel(QImage image){
   // std::cout << "start of method" << std::endl;
    noOfImages++;

    QLabel * imageLabel = new QLabel();
    imageLabel->setPixmap(QPixmap::fromImage(image.rgbSwapped()));
    //imagePanel->addWidget(imageLabel,noOfImages,0,Qt::AlignCenter);
    //scrollArea->addScrollBarWidget(imageLabel,Qt::AlignCenter);
    imageList->addWidget(imageLabel,Qt::AlignRight);
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
    //scrollArea->addScrollBarWidget(imageLabel,Qt::AlignCenter);
    imageList->addWidget(imageLabel,Qt::AlignRight);
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

bool MainWindow::compareMatImages(){
//compare pics
    std::cout << "compare image method" << std::endl;
    std::cout <<  matOriginal.rows << "," << matOriginal.cols <<std::endl;
    std::cout <<  matPrevious.rows << "," << matPrevious.cols <<std::endl;
    if (matOriginal.rows != matPrevious.rows || matOriginal.cols != matPrevious.cols){
        std::cout << "image sizes do not match" << std::endl;
        return false;
    }
    matOriginal.copyTo(matEdited);

    if (matEdited.empty()){
        return false;
    }

    std::cout << "compare image prelims passed" << std::endl;
    std::cout << "Using send of " << threshold << std::endl;
    for (int i = 0; i < matPrevious.rows; i++){
        for (int j = 0; j < matPrevious.cols; j++){

            int diff = std::abs((int)matPrevious.at<uchar>(i,j) - (int)matOriginal.at<uchar>(i,j));

            /*if ((int)matPrevious.at<uchar>(i,j) != (int)matOriginal.at<uchar>(i,j)){
                std::cout << (int)matPrevious.at<uchar>(i,j) << std::endl;
                std::cout << (int)matOriginal.at<uchar>(i,j) << std::endl;
            }*/

            if (diff > threshold){
                //std::cout << "found pixel" << std::endl;
                matEdited.at<cv::Vec3b>(i,j)[0] = 0;
                matEdited.at<cv::Vec3b>(i,j)[1] = 0;
                matEdited.at<cv::Vec3b>(i,j)[2] = 255;
            }

            /*if (diff > 0){
                std::cout << diff << std::endl;
            }*/

            //std::cout << (int)image.at<cv::Vec3b>(i,j)[0] << " " << (int)image.at<cv::Vec3b>(i,j)[1] << " " << (int)image.at<cv::Vec3b>(i,j)[2] << ",";
        }



    }
    std::cout << "compare done" << std::endl;
    return true;


}


bool MainWindow::compareScanLine(){

}

bool MainWindow::compareQImages(){
    qImgOriginal = QImage((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    qImgPrevious = QImage((uchar*)matPrevious.data, matPrevious.cols, matPrevious.rows, matPrevious.step, QImage::Format_RGB888);
    qImgEdited = QImage((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);


    std::cout << qImgOriginal.height() << " , " << qImgOriginal.width() << std::endl;
    std::cout << "Using send of " << threshold << std::endl;

    for (int i = 0; i < qImgOriginal.height(); i++){
        for (int j = 0; j < qImgOriginal.width(); j++){


            QRgb col = qImgOriginal.pixel(j,i);
            QColor c1 = QColor(col);
            int totalOriginal = c1.green() + c1.blue() + c1.red() + c1.black() + c1.cyan() + c1.magenta() + c1.yellow();
            //totalOriginal = totalOriginal;


            QRgb col2 = qImgPrevious.pixel(j,i);

            QColor c2 = QColor(col2);
            int totalOld = c2.green() + c2.blue() + c2.red() + c2.black() + c2.cyan() + c2.magenta() + c2.yellow();
            //totalOld = totalOld;
            //std::cout << totalOriginal << "," << totalOld << std::endl;


            int diff = std::abs(totalOld - totalOriginal);
            //std::cout << diff << std::endl;

            /*if (qImgPrevious.pixel(j,i) != qImgOriginal.pixel(j,i)){
                //std::cout << "found pixel" << std::endl;
                qImgEdited.setPixel(j,i,QRgb(redColor));
            }*/


            if (diff > threshold){
                //std::cout << "found pixel" << std::endl;
                qImgEdited.setPixel(j,i,QRgb(redColor));
            }


        }



    }
    std::cout << "compare done" << std::endl;
    return true;

   // mainImage->setPixmap(QPixmap::fromImage(qImgEdited.rgbSwapped()));

   // mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);
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



    addImageToPanel(QImage((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888));
    setSensitivity();
    //handle show next button being pressed
    //first handle keyboard events (optional access), needs to be done in the update loop, otherwise this will be called by pressing the button
    std::cout << "next shot slot" << std::endl;
    //we back up the old image
    matOriginal.copyTo(matPrevious);
    if (matPrevious.empty()){
        return;
    }

    //we read in a new image
    videocap.read(matOriginal);
    //if nothing is coming through
    if (matOriginal.empty()){
        return;
    }

    bool useMat = false;
    if (useMat){

        if (compareMatImages()){
            qImgEdited = QImage((uchar*)matEdited.data, matEdited.cols, matEdited.rows, matEdited.step, QImage::Format_RGB888);

            mainImage->setPixmap(QPixmap::fromImage(qImgEdited.rgbSwapped()));

            mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);
        }
        else{
            std::cout << "image compare failed" << std::endl;
        }
    }
    else{
        if (compareQImages()){

            mainImage->setPixmap(QPixmap::fromImage(qImgEdited.rgbSwapped()));

            mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);
        }
        else{
            std::cout << "image compare failed" << std::endl;
        }

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


void MainWindow::createMenuBar(){
    //FILE MENU
    fileMenu = new QMenu(tr("&File"),this);
    fileMenu->addAction(saveSessionAction);
    fileMenu->addAction(newSessionAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    //MODE MENU
    modeMenu = new QMenu(tr("&Mode"),this);
    modeMenu->addAction(shootingModeAction);
    modeMenu->addAction(liveVideoModeAction);

    //HELP MENU
    helpMenu = new QMenu(tr("&Help"),this);
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(tutorialLinkAction);
    helpMenu->addAction(contactAction);

    //add all menus in
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(modeMenu);
    menuBar()->addMenu(helpMenu);

}

void MainWindow::createActions(){
    //EXIT ACTION
    exitAction = new QAction(tr("&Exit"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exitApp()));

    //SAVE SESSION ACTION
    saveSessionAction = new QAction(tr("&Save Session"),this);
    saveSessionAction->setShortcut(tr("Ctrl+S"));
    connect(saveSessionAction, SIGNAL(triggered()), this, SLOT(saveSession()));

    //NEW SESSION ACTION
    newSessionAction = new QAction(tr("&New Session"),this);
    newSessionAction->setShortcut(tr("Ctrl+N"));
    //connect(newSessionAction, SIGNAL(triggered()), this, SLOT()));


    //SHOOT MODE ACTION
    shootingModeAction = new QAction(tr("&Shooting Mode"),this);
    connect(shootingModeAction, SIGNAL(triggered()), this, SLOT(on_ShootingBtnPress()));

    //LIVE MODE ACTION
    liveVideoModeAction = new QAction(tr("&Live Video Mode"),this);
    connect(liveVideoModeAction, SIGNAL(triggered()), this, SLOT(on_LiveFeedBtnPress()));

    //ABOUT ACTION
    aboutAction = new QAction(tr("&About"),this);
    //connect(aboutAction, SIGNAL(triggered()), this, SLOT()));

    //TUT ACTION
    tutorialLinkAction = new QAction(tr("&Tutorials"),this);
    //connect(tutorialLinkAction, SIGNAL(triggered()), this, SLOT()));

    //CONTACT ACTION
    contactAction = new QAction(tr("&Contatc Us"),this);
    //connect(contactAction, SIGNAL(triggered()), this, SLOT()));



}


//saves a specific image
void MainWindow::saveIdivImage(){
    QString savePath = QFileDialog::getSaveFileName(this,tr("Save File"), "", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));
    //images[i]->pixmap()->save(savePath);
}

void MainWindow::saveSession(){





    //dialog
    QString savePath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home",  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    //std::cout << savePath.toStdString() << std::endl;

    //std::cout << savePath.toStdString() << std::endl;
    for (int i = 0; i < noOfImages; i++){
        QString fullPath = savePath;
        fullPath.append("/Shot ");
        fullPath.append(QString::number(i));
        fullPath.append(".png");

        images[i]->pixmap()->save(fullPath);

    }



}


void MainWindow::newSession(){
    images.clear();
    connectToCamera();
    //scrollArea->
}

void MainWindow::exitApp(){
    QApplication::quit();
}


void MainWindow::setSensitivity(){
    threshold = textBox->text().toInt();
}
