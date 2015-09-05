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

    QPushButton * liveFeed = new QPushButton("Live Feed");
    QPushButton * shootMode = new QPushButton("Shooting Mode");
    QPushButton * nextShot = new QPushButton("Show Next Shot");
    QPixmap currentImage("c:\\Users\\SMNM\\Pictures\\Default\\orca.jpg");
    QLabel * imageLabel2 = new QLabel();
    imageLabel2->setPixmap(currentImage);


    mainPanel->addWidget(liveFeed,0,0);
    mainPanel->addWidget(shootMode,0,1);
    mainPanel->addWidget(imageLabel2,1,0,1,2);
    mainPanel->addWidget(nextShot,2,0,1,2);


    ///PICTURES
    ///
    QPixmap image1("c:\\Users\\SMNM\\Pictures\\Default\\beach.jpg");
    QLabel * imageLabel = new QLabel();
    imageLabel->setPixmap(image1);
    imagePanel->addWidget(imageLabel);

    rightWidget->setLayout(imagePanel);
    leftWidget->setLayout(mainPanel);

    gridLayout->addWidget(leftWidget,0,0);
    gridLayout->addWidget(rightWidget,0,1);

    mainWidget->setLayout(gridLayout);


    //imageView->setLayout(imagePanel);
    //gridLayout->addWidget(imageView,0,1);
    setCentralWidget(mainWidget);




}



void MainWindow::setUpLayout(){

}

MainWindow::~MainWindow()
{
    delete ui;
}

