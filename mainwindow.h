#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

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

private:
    Ui::MainWindow *ui;
    QWidget * mainWidget;
    QWidget * leftWidget;
    QWidget * rightWidget;
    QGridLayout * gridLayout;
    QGridLayout * imagePanel;
    QGridLayout * mainPanel;

};

#endif // MAINWINDOW_H

