#include "include/LogsWidget.hpp"

LogsWidget::LogsWidget(QWidget *parent) : QWidget(parent)
{
    m_MainLayout = new QVBoxLayout;
    this->setLayout(m_MainLayout);
}

void LogsWidget::Log(const QString &message)
{
    QString formattedTime =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QString log_message = formattedTime + "-Drone: " + message;

    auto log_label = new QLabel{log_message, this};

    m_MainLayout->addWidget(log_label);
}

LogsWidget::~LogsWidget() = default;
