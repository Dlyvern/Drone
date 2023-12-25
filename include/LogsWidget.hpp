#ifndef LOGS_WIDGET_HPP
#define LOGS_WIDGET_HPP


#include <QVBoxLayout>
#include "QWidget"
#include "QLabel"
#include "QDateTime"

class LogsWidget : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout *m_MainLayout{nullptr};
public:
    explicit LogsWidget(QWidget* parent = nullptr);

    virtual ~LogsWidget();

public slots:

    void Log(const QString& message);
};

#endif //LOGS_WIDGET_HPP
