#include "include/mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    m_Drone = new Drone();

    auto central_widget = new QWidget;

    auto main_layout = new QVBoxLayout{central_widget};

    main_layout->addWidget(m_Drone);

    this->setCentralWidget(central_widget);

    this->resize({1920, 1080});
}

MainWindow::~MainWindow() = default;

