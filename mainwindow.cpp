#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    QWidget * widget = new QWidget(this);
    QGridLayout * gridLayout = new QGridLayout(widget);
    QGridLayout * imagePanel = new QGridLayout();
    QGridLayout * mainPanel = new QGridLayout();



    QPushButton * liveFeed = new QPushButton("Live Feed");
    QPushButton * shootMode = new QPushButton("Shooting Mode");
    QPushButton * nextShot = new QPushButton("Show Next Shot");
    QWidget * imageView = new QWidget(this);

    mainPanel->addWidget(liveFeed,0,0);
    mainPanel->addWidget(shootMode,0,1);
    mainPanel->addWidget(imageView,1,0,1,2);
    mainPanel->addWidget(nextShot,2,0,1,2);
    gridLayout->addItem(mainPanel,0,0);
    gridLayout->addItem(imagePanel,0,1);
    widget->setLayout(gridLayout);
    setCentralWidget(widget);
}

void MainWindow::setUpLayout(){
    QWidget * widget = new QWidget(this);
    QGridLayout * gridLayout = new QGridLayout(widget);
    QGridLayout * imagePanel = new QGridLayout(widget);
    QGridLayout * mainPanel = new QGridLayout(widget);

    gridLayout->addItem(mainPanel,0,0);
    gridLayout->addItem(imagePanel,0,1);

    QPushButton * liveFeed = new QPushButton("Live Feed");
    QPushButton * shootMode = new QPushButton("Shooting Mode");
    QPushButton * nextShot = new QPushButton("Show Next Shot");
    QWidget * imageView = new QWidget(this);

    mainPanel->addWidget(liveFeed,0,0);
    mainPanel->addWidget(shootMode,0,1);
    mainPanel->addWidget(imageView,1,0,1,2);
    mainPanel->addWidget(nextShot,2,0,1,2);
    setCentralWidget(widget);
}



MainWindow::~MainWindow()
{
    delete ui;
}

