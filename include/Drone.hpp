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

#include "QTableWidget"

#include "QScrollArea"
#include "LogsWidget.hpp"

class Drone : public QWidget
{
    Q_OBJECT
private:
    QPushButton* m_ConnectButton{nullptr};
    QPushButton* m_DisconnectButton{nullptr};
    QLabel* m_InfoConnected{nullptr};
    QProgressBar* m_CommandProgress{nullptr};
    QTableWidget* m_ModulesTable{nullptr};

    QTcpSocket *m_TCPSocket{nullptr};
    QListWidget* m_ActionsList{nullptr};
    QHostAddress m_TCPIpAddress{"127.0.0.1"};
    int m_TCPPort{8080};
    QString m_Name{"Drone"};
    int m_Level{2};
    int m_NumberOfSentMessages{0};

    QVBoxLayout *m_Layout{nullptr};

    bool m_IsConnected{false};

    int m_MaxNumberOfConnectionAttempts{10};

    QByteArray MakeMessageForServer(QJsonObject &&data);

private slots:
    void Connect();

    void ReadMessageFromServer();

    void SendMission(QListWidgetItem *item);

    void Disconnect();


public:

    explicit Drone();
    void Start();

signals:
    void Log(const QString &message);

};

#endif //DRONE_HPP
