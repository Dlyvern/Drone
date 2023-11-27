#include "include/Packet.hpp"

Packet::Packet(const QByteArray &data)
{
    Unpack(data);
}

bool Packet::Unpack(const QByteArray &data)
{
    QDataStream stream(data);

    stream.setByteOrder(QDataStream::LittleEndian);

    stream >> m_Header;

//    if(m_Header != 0xAA55)
//        return false;

    stream >> m_Type;

    stream >> m_Number;

    stream >> m_LengthOfData;

    QByteArray json_data = data.mid(stream.device()->pos(), m_LengthOfData);

    QJsonDocument jsonDoc = QJsonDocument::fromJson(json_data);

    m_Data = jsonDoc.object();

    if(!stream.device()->seek(stream.device()->pos() + m_LengthOfData))
        qDebug() << "Got broken message";

    stream >> m_CRC;

    stream >> m_CRC;

    return true;
}

QByteArray Packet::Pack() const
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    stream << m_Header;
    stream << m_Type;
    stream << m_Number;

    QJsonDocument jsonDoc(m_Data);
    QByteArray jsonData = jsonDoc.toJson();
    quint16 jsonLength = jsonData.length();

    stream << jsonLength;

    stream.writeRawData(jsonData.data(), jsonLength);
    stream << m_CRC;

    return packet;
}

Packet::Packet(qint16 header, quint8 type, quint16 number, quint16 lengthOfData, QJsonObject &&data, quint16 crc) :
        m_Header{header},
        m_Type{type},
        m_Number{number},
        m_LengthOfData{lengthOfData},
        m_Data{std::move(data)},
        m_CRC{crc}
{

}


const QJsonObject Packet::GetData() const
{
    return m_Data;
}

Packet::Packet() = default;

