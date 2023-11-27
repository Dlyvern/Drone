#ifndef DRONE_HPP
#define DRONE_HPP

#include "QWidget"
#include "QTcpSocket"
#include "QHostAddress"
#include "QPushButton"
#include "QListWidget"
#include <QJsonObject>
#include "QMenu"
#include "Packet.hpp"
#include <QJsonDocument>
#include <QVBoxLayout>
#include "QTimer"
#include "thread"
#include "QLabel"
#include "QProgressBar"

class Drone : public QWidget
{
    Q_OBJECT
private:
    QPushButton* m_ConnectButton{nullptr};
    QPushButton* m_DisconnectButton{nullptr};
    QLabel* m_InfoConnected{nullptr};
    QProgressBar* m_CommandProgress{nullptr};

    QTcpSocket *m_TCPSocket{nullptr};
    QListWidget *m_LogMessages{nullptr};
    QListWidget* m_ActionsList{nullptr};
    QHostAddress m_TCPIpAddress{"127.0.0.1"};
    int m_TCPPort{8080};
    QString m_Name{"Drone"};
    int m_Level{2};
    int m_NumberOfSentMessages{0};

    QVBoxLayout *m_Layout{nullptr};

    bool m_IsConnected{false};

    int m_MaxNumberOfConnectionAttempts{10};

    void Log(const QString &message);

    QByteArray MakeMessageForServer(QJsonObject &&data);

private slots:
    void Connect();

    void ReadMessageFromServer();

    void SendMission(QListWidgetItem *item);


public:

    explicit Drone();

    void Start();

};

#endif //DRONE_HPP
