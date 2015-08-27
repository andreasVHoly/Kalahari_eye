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

    setUpLayout();




}

void MainWindow::setUpLayout(){
    QPushButton * liveFeed = new QPushButton("Live Feed");
    QPushButton * shootMode = new QPushButton("Shooting Mode");
    QPushButton * nextShot = new QPushButton("Show Next Shot");
    QWidget * imageView = new QWidget(this);

    mainPanel->addWidget(liveFeed,0,0);
    mainPanel->addWidget(shootMode,0,1);
    mainPanel->addWidget(imageView,1,0,1,2);
    mainPanel->addWidget(nextShot,2,0,1,2);

    gridLayout->addItem(mainPanel);
    gridLayout->addItem(imagePanel);
    mainWidget->setLayout(gridLayout);
    setCentralWidget(mainWidget);
}



MainWindow::~MainWindow()
{
    delete ui;
}

