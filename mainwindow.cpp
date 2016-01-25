#include "mainwindow.h"
#include "ui_mainwindow.h"


//THis version of the program is intended to not compare shots but just feature the next shot mode and live mode with being able to view the shots


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    std::cout << "Program Starting..." << std::endl;

    images.clear();
    currentState = SystemState::mode_novid;
    noOfImages = 0;
    //widgets
    mainWidget = new QWidget(this);


    //RIGHT WIDGET, holds the seperate images
    rightWidget = new QWidget(mainWidget);
    rightWidget->setStyleSheet("*{background-color:rgb(0,0,0);}");


    scrollArea = new QScrollArea();
    imageList = new QVBoxLayout(scrollArea);
    scrollArea->setStyleSheet("*{background-color:rgb(0,0,0);}");

    scrollArea->setWidget(imageList->widget());
    scrollArea->setWidgetResizable(true);


    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setAlignment(Qt::AlignCenter);







    //LEFT WIDGET,intended to hold the main view and the buttons
    leftWidget = new QWidget(mainWidget);
    leftWidget->setStyleSheet("*{background-color:rgb(100,50,50);}");


    //the differant layouts for the parts of the screen
    mainLayout = new QGridLayout(mainWidget);
    imagePanel = new QGridLayout(rightWidget);
    mainPanel = new QGridLayout(leftWidget);
    mainImage = new QLabel();


    ///BUTTONS
    addInButtons();

    //setting up the widgets and their layouts
    imagePanel->addWidget(scrollArea);
    rightWidget->setLayout(imagePanel);
    leftWidget->setLayout(mainPanel);

    mainLayout->addWidget(leftWidget,0,0);
    mainLayout->addWidget(rightWidget,0,1);

    mainWidget->setLayout(mainLayout);


    setCentralWidget(mainWidget);
    //creating the actions for different elements
    createActions();
    createMenuBar();

    //connecting to the camera
    bool debugCamStart = true;
    if (debugCamStart){
        connectToCamera();
    }


     std::cout << "end main..." << std::endl;


}


void MainWindow::connectToCamera(){
    camera = new andreasvh::CameraLink("admin", "1234", 100, 150, "8080");

    if (!camera->setUpConnection()){
        std::cout << "Error connection to camera" << std::endl;
    }
    else{
        std::cout << "Connection to camera successful" << std::endl;
        currentState = SystemState::mode_live;

    }

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

    //assign clicking and pressing actions to nessesary slots for each button
    liveFeed = new QPushButton("Live Feed");
    connect(liveFeed, SIGNAL(pressed()), this, SLOT(on_LiveFeedBtnPress()));
    shootMode = new QPushButton("Shooting Mode");
    connect(shootMode, SIGNAL(pressed()), this, SLOT(on_ShootingBtnPress()));
    nextShot = new QPushButton("Show Next Shot");
    connect(nextShot, SIGNAL(pressed()), this, SLOT(on_ShowNextShotBtnPress()));


    //add in the buttons
    mainPanel->addWidget(liveFeed,0,0);
    mainPanel->addWidget(shootMode,0,1);
    mainPanel->addWidget(nextShot,2,0,1,2);

}

///
/// \brief MainWindow::addImageToPanel
/// \param image
///
void MainWindow::addImageToPanel(QImage image){
    noOfImages++;

    QLabel * imageLabel = new QLabel();
    imageLabel->setPixmap(QPixmap::fromImage(image.rgbSwapped()));
    imageList->addWidget(imageLabel,Qt::AlignRight);
    if (images.empty()){
        images.reserve(noOfImages);
    }
    images.push_back(imageLabel);

}



///
/// \brief MainWindow::addImageFromDrive
/// adding an image from the drive, more used in testing
/// \param path
///
void MainWindow::addImageFromDrive(std::string path){
    noOfImages++;
    QPixmap image(path.c_str());
    ClickableLabel * imageLabel = new ClickableLabel(image, this);


    connect(imageLabel, SIGNAL(pressed()), this, SLOT(on_ImagePress()));
    imageList->addWidget(imageLabel,Qt::AlignRight);
    if (images.empty()){
        images.reserve(noOfImages);
    }
    images.push_back(imageLabel);
}

///
/// \brief MainWindow::updateFeed
/// updates the display if currently in live mode and otherwise just refereshes the incoming stream
/// this is called as often as possible
///
void MainWindow::updateFeed(){
    //update the main feed
    //simple video update


    cv::Mat tempMat;
    //we want to simply update the stream if we are not in live mode, thus returning here
    if (currentState != SystemState::mode_live){
        videocap.read(tempMat);
        return;
    }

    //if we are in live mode we would have made it until here
    //we read in the new data to matOriginal
    videocap.read(matOriginal);

    //if nothing is coming through
    if (matOriginal.empty()){
        return;
    }

    //we make a new qimage out of it and assign it to the main image
    qImgOriginal = QImage((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);

    mainImage->setPixmap(QPixmap::fromImage(qImgOriginal.rgbSwapped()));

    mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);

}

///
/// \brief MainWindow::compareMatImages

/// \return
///
bool MainWindow::compareMatImages(){
//compare pics
    std::cout << "compare image method" << std::endl;
    std::cout <<  matOriginal.rows << "," << matOriginal.cols <<std::endl;
    std::cout <<  matPrevious.rows << "," << matPrevious.cols <<std::endl;
    if (matOriginal.rows != matPrevious.rows || matOriginal.cols != matPrevious.cols){
        std::cout << "image sizes do not match" << std::endl;
        return false;
    }

    matEdited = matOriginal.clone();



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

    QImage temp1 = QImage((uchar*)matEdited.data, matEdited.cols, matEdited.rows, matEdited.step, QImage::Format_RGB888);
    picture3->setPixmap(QPixmap::fromImage(temp1.rgbSwapped()));
    mainPanel->addWidget(picture3,1,2,Qt::AlignCenter);
    return true;


}


///
/// \brief MainWindow::compareQImages
/// NOT USED IN THIS VERSION
/// \return
///
bool MainWindow::compareQImages(){
    //qImgOriginal = QImage((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    //qImgPrevious = QImage((uchar*)matPrevious.data, matPrevious.cols, matPrevious.rows, matPrevious.step, QImage::Format_RGB888);


    qImgPrevious = qImgOriginal.copy();

    //qImgOriginal = QImage((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    qImgEdited = QImage((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);

    picture1->setPixmap(QPixmap::fromImage(qImgPrevious.rgbSwapped()));
    mainPanel->addWidget(picture1,1,0,Qt::AlignCenter);

    picture2->setPixmap(QPixmap::fromImage(qImgOriginal.rgbSwapped()));
    mainPanel->addWidget(picture2,1,1,Qt::AlignCenter);

    picture3->setPixmap(QPixmap::fromImage(qImgEdited.rgbSwapped()));
    mainPanel->addWidget(picture3,1,2,Qt::AlignCenter);




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

///
/// \brief MainWindow::keyPressEvent
/// HANDLES key press events if we need any
/// \param e
///
void MainWindow::keyPressEvent(QKeyEvent * e){

}


///
/// \brief MainWindow::on_ShowNextShotBtnPress
/// slot for the next button being pressed
///
void MainWindow::on_ShowNextShotBtnPress(){
    //if we are not in shooting mode we dont want this button to react
    if (currentState != 0){
        return;
    }

    addImageToPanel(QImage((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888));

    //handle show next button being pressed
    //first handle keyboard events (optional access), needs to be done in the update loop, otherwise this will be called by pressing the button ----- THIS IS ONLY FOR DESKTOP VERSION?
    //we back up the old image

    matPrevious = matOriginal.clone();
    if (matPrevious.empty()){
        return;
    }

    //we read in a new image
    videocap.read(matOriginal);
    //if nothing is coming through
    if (matOriginal.empty()){
        return;
    }

    qImgOriginal = QImage((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);

    mainImage->setPixmap(QPixmap::fromImage(qImgOriginal.rgbSwapped()));

    mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);


    //WE NO LONGER COMPARE IN THIS VERSION THUS THIS IS COMMENTED OUT
   /* bool useMat = false;
    if (useMat){

        //to check image comaprison
        //PREVIOUS IMAGE
        bool compareCheck = true;
        if (compareCheck){
            QImage temp0 = QImage((uchar*)matPrevious.data, matPrevious.cols, matPrevious.rows, matPrevious.step, QImage::Format_RGB888);
            picture1->setPixmap(QPixmap::fromImage(temp0.rgbSwapped()));
            mainPanel->addWidget(picture1,1,0,Qt::AlignCenter);


            //ORIGINAL IMAGE (*NEW)
            QImage temp1 = QImage((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
            picture2->setPixmap(QPixmap::fromImage(temp1.rgbSwapped()));
            mainPanel->addWidget(picture2,1,1,Qt::AlignCenter);
        }





        if (compareMatImages()){
            qImgEdited = QImage((uchar*)matEdited.data, matEdited.cols, matEdited.rows, matEdited.step, QImage::Format_RGB888);

            mainImage->setPixmap(QPixmap::fromImage(qImgEdited.rgbSwapped()));

            mainPanel->addWidget(mainImage,2,0,1,2,Qt::AlignCenter);
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
*/


}



///
/// \brief MainWindow::on_ShootingBtnPress
/// shooting mode response
///
void MainWindow::on_ShootingBtnPress(){
//handle shooting button being pressed
    //already in shooting mode
    if (currentState == 0){
        return;
    }
    currentState = SystemState::mode_shooting;
    addImageToPanel(qImgOriginal);
}

///
/// \brief MainWindow::on_LiveFeedBtnPress
/// live mode response
///
void MainWindow::on_LiveFeedBtnPress(){
//handle live feed button being pressed
    if (currentState == 1){
        return;
    }
    currentState = SystemState::mode_live;

}

///
/// \brief MainWindow::on_ImagePress
/// response to pressing on an image
///
void MainWindow::on_ImagePress(){

    if (QObject::sender()->isWidgetType()){
        const QPixmap * map = qobject_cast<ClickableLabel*>(QObject::sender())->pixmap();
        QPixmap &temp = const_cast<QPixmap&>(*map);
        mainImage->setPixmap(temp);
        mainPanel->addWidget(mainImage,1,0,1,2,Qt::AlignCenter);
    }

    currentState = SystemState::mode_imageView;



 }

void MainWindow::removeImage(std::string path){

}

void MainWindow::setImageAmount(int size){
    noOfImages = size;
}


MainWindow::~MainWindow()
{
    delete ui;
    delete camera;
}

///
/// \brief MainWindow::createMenuBar
/// populates the bar of the window, is this needed for Android?
///
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

///
/// \brief MainWindow::createActions
/// creates the actions for the menu bar items
///
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


///
/// \brief MainWindow::saveSession
/// saves all images that were taken
/// beta stage
///
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

///
/// \brief MainWindow::newSession
/// resets the session
///
void MainWindow::newSession(){
    images.clear();
    connectToCamera();
}

void MainWindow::exitApp(){
    QApplication::quit();
}



