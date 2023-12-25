#include "include/Drone.hpp"

Drone::Drone()
{
    auto logs_widget = new LogsWidget{this};
    auto scroll_area = new QScrollArea;
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(logs_widget);

    connect(this, &Drone::Log, logs_widget, &LogsWidget::Log);

    m_ModulesTable = new QTableWidget{this};

    m_ModulesTable->setHorizontalHeaderLabels({"Name", "Status"});

    m_Layout = new QVBoxLayout;

    m_ActionsList= new QListWidget{this};

    m_CommandProgress = new QProgressBar{this};

    m_CommandProgress->setRange(0, 100);

    m_ActionsList->addItem("Robot - DoDiagnostic");
    m_ActionsList->addItem("Robot - Enable");
    m_ActionsList->addItem("Robot - Disable");
    m_ActionsList->addItem("Robot - LongTest");
    m_ActionsList->addItem("Recorder - Enable");
    m_ActionsList->addItem("Recorder - Disable");
    m_ActionsList->addItem("Copyist - CopyFromBagsToFlash");
    m_ActionsList->addItem("RtspCamera - Enable");

    m_ActionsList->resize({500, 200});

    connect(m_ActionsList, &QListWidget::itemClicked, this, &Drone::SendMission);

    auto layout = new QHBoxLayout;

    m_ConnectButton = new QPushButton(this);
    m_ConnectButton->setText("Connect to server");

    m_DisconnectButton = new QPushButton(this);
    m_DisconnectButton->setText("Disconnect from server");

    connect(m_ConnectButton, &QPushButton::clicked, this, &Drone::Connect);
    connect(m_DisconnectButton, &QPushButton::clicked, this, &Drone::Disconnect);

    layout->addWidget(m_ConnectButton);
    layout->addWidget(m_DisconnectButton);

    m_Layout->addWidget(m_ModulesTable);
    m_Layout->addWidget(m_ActionsList);
    m_Layout->addWidget(m_CommandProgress);
    m_Layout->addLayout(layout);
    m_Layout->addWidget(scroll_area);

    this->setLayout(m_Layout);
}

void Drone::SendMission(QListWidgetItem *item)
{
    if(!m_IsConnected)
    {
        emit Log("Not connected to server");
        return;
    }

    QJsonObject data;

    data["type"] = "cmd";

    QString target_name = item->text().split(" - ").at(0);
    QString operation_name = item->text().split(" - ").at(1);

//    data["cmd"] = "Robot.Enable|24";
    data["cmd"] = target_name + "." + operation_name + "|24";

    QByteArray container = MakeMessageForServer(std::move(data));

    m_TCPSocket->write(container);

    m_TCPSocket->waitForBytesWritten();

    m_CommandProgress->setValue(1);
}


void Drone::Connect()
{     
    if(m_IsConnected)
    {
        emit Log("Already connected");
        return;
    }

    m_TCPSocket = new QTcpSocket(this);

    connect(m_TCPSocket, &QTcpSocket::readyRead, this, &Drone::ReadMessageFromServer);

    m_TCPSocket->connectToHost(m_TCPIpAddress, m_TCPPort);

    if(!m_TCPSocket->waitForConnected())
    {

        emit Log("Could not connect to server");
        return;
    }

    m_IsConnected = true;


    QJsonObject init_data;

    init_data["name"] = m_Name;
    init_data["level"] = m_Level;
    init_data["ip"] = "198.27.54.20";
    init_data["tcp"] = true;
    init_data["type"] = "connect";
    init_data["msg"] = "Hello from Drone";

    QByteArray init_container = MakeMessageForServer(std::move(init_data));

    m_TCPSocket->write(init_container);

    m_TCPSocket->waitForBytesWritten();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    QJsonObject get_op_data;

    get_op_data["type"] = "get_op";

    QByteArray op_container = MakeMessageForServer(std::move(get_op_data));

    m_TCPSocket->write(op_container);

    m_TCPSocket->waitForBytesWritten();

    emit Log("Connected to TCP/IP Server");
}

QByteArray Drone::MakeMessageForServer(QJsonObject &&data)
{
    Packet packet_to_send(0xAA55, 0x01, 0x1234, 0, std::move(data), 0xFFFF);

    QByteArray container = packet_to_send.Pack();

    return container;
}

void Drone::ReadMessageFromServer()
{
    QByteArray dataForInitialization = m_TCPSocket->readAll();

    Packet packet;

    if(!packet.Unpack(dataForInitialization))
    {
        emit Log("Got a broken message");
    }

    const QJsonObject &data = packet.GetData();

    if(data["type"] == "result")
    {
        QString result = "Result: ";

        if(data["res"].toBool())
            result += "true\n";
        else
            result += "false\n";


        result += " Result arguments: ";

        result +=  data["res_arg"].toString();

        emit Log(result);

        m_CommandProgress->setValue(100);
    }

    else if(data["type"] == "message")
    {
        QString message{" "};

        message = data["msg"].toString();

        emit Log(message);
    }

    else if(data["type"] == "feedback")
    {
        int progress = data["progress"].toInt();
        m_CommandProgress->setValue(progress);

        if(m_CommandProgress->value() >= 90)
        {
            m_CommandProgress->setValue(70);
        }
    }
    else emit Log("Unknown type of message from server");
}

void Drone::Disconnect()
{
    if(!m_IsConnected)
    {
        emit Log("Not connected to server");
        return;
    }

    QJsonObject data;

    data["type"] = "cmd";
    data["cmd"] = "disconnect";

    QByteArray container = MakeMessageForServer(std::move(data));

    m_TCPSocket->write(container);

    m_TCPSocket->waitForBytesWritten();

    m_TCPSocket->disconnect();

    m_IsConnected = false;
    emit Log("Disconnected from server");

}

