#include "include/mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    m_Drone = new Drone();

    setCentralWidget(m_Drone);
    this->resize({1200, 720});
}

MainWindow::~MainWindow()
{
    delete ui;
}

