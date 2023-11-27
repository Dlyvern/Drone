#ifndef PACKET_HPP
#define PACKET_HPP

#include <QByteArray>
#include <QDataStream>
#include <QJsonObject>
#include "QJsonDocument"

class Packet
{
private:
    qint16 m_Header;
    quint8 m_Type;
    quint16 m_Number;
    quint16 m_LengthOfData;
    QJsonObject m_Data;
    quint16 m_CRC;

public:
    Packet(qint16 header, quint8 type, quint16 number, quint16 lengthOfData, QJsonObject&& data, quint16 crc);

    Packet();

    explicit Packet(const QByteArray& data);

    [[nodiscard]] const QJsonObject GetData() const;

    bool Unpack(const QByteArray& data);

    [[nodiscard]] QByteArray Pack() const;
};

#endif //PACKET_HPP
