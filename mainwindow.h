#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <string>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setUpLayout();
    void addInButtons();
    void addImage(std::string path);
    void removeImage(std::string path);
    void setImageAmount(int size);

private:
    Ui::MainWindow *ui;
    QWidget * mainWidget;
    QWidget * leftWidget;
    QWidget * rightWidget;
    QGridLayout * gridLayout;
    QGridLayout * imagePanel;
    QGridLayout * mainPanel;
    std::vector<QLabel*> images;
    QLabel * mainImage;
    int noOfImages;

};

#endif // MAINWINDOW_H

