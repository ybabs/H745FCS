#include "inc/frame.h"
#include <QDebug>


Frame::Frame(QObject *parent) : QObject(parent)
{

}
Frame::Frame(quint8 cmd, quint8 data, QObject *parent ): QObject(parent)
{
    m_packet[FRAME_START_INDEX] = FRAME_PREAMBLE_A;
    m_packet[FRAME_PREAMBLE_2_INDEX] = FRAME_PREAMBLE_B;
    m_packet[CMD_CODE_INDEX] = cmd;
    m_packet[DATA_LEN_INDEX] = 1;
    m_packet[DATA_START_INDEX] = data;
    m_packet+=CalculateChecksum(m_packet.size()-1);
}
Frame::Frame(quint8 cmd, qint8 data, QObject *parent ): QObject(parent)
{
    m_packet[FRAME_START_INDEX] = FRAME_PREAMBLE_A;
    m_packet[FRAME_PREAMBLE_2_INDEX] = FRAME_PREAMBLE_B;
    m_packet[CMD_CODE_INDEX] = cmd;
    m_packet[DATA_LEN_INDEX] = 1;
    m_packet[DATA_START_INDEX] = quint8(data);
    m_packet+=CalculateChecksum(m_packet.size()-1);
}
Frame::Frame(quint8 cmd, quint16 data, QObject *parent ): QObject(parent)
{
    m_packet[FRAME_START_INDEX] = FRAME_PREAMBLE_A;
    m_packet[FRAME_PREAMBLE_2_INDEX] = FRAME_PREAMBLE_B;
    m_packet[CMD_CODE_INDEX] = cmd;
    m_packet[DATA_LEN_INDEX] = 2;
    m_packet[DATA_START_INDEX] = quint8((data & 0xFF00) >> 8);
    m_packet[DATA_START_INDEX + 1] = quint8(data & 0x00FF);
    m_packet+=CalculateChecksum(m_packet.size()-1);
}
Frame::Frame(quint8 cmd, qint16 data, QObject *parent ): QObject(parent)
{
    m_packet[FRAME_START_INDEX] = FRAME_PREAMBLE_A;
    m_packet[FRAME_PREAMBLE_2_INDEX] = FRAME_PREAMBLE_B;
    m_packet[CMD_CODE_INDEX] = cmd;
    m_packet[DATA_LEN_INDEX] = 2;
    m_packet[DATA_START_INDEX] = quint8((data & 0xFF00) >> 8);
    m_packet[DATA_START_INDEX + 1] = quint8(data & 0x00FF);
    m_packet+=CalculateChecksum(m_packet.size()-1);
}
Frame::Frame(quint8 cmd, quint32 data, QObject *parent ): QObject(parent)
{
    m_packet[FRAME_START_INDEX] = FRAME_PREAMBLE_A;
    m_packet[FRAME_PREAMBLE_2_INDEX] = FRAME_PREAMBLE_B;
    m_packet[CMD_CODE_INDEX] = cmd;
    m_packet[DATA_LEN_INDEX] = 4;
    m_packet[DATA_START_INDEX] = quint8((data & 0xFF000000) >> 24);
    m_packet[DATA_START_INDEX + 1] = quint8((data & 0x00FF0000) >> 16);
    m_packet[DATA_START_INDEX + 2] = quint8((data & 0x0000FF00) >> 8);
    m_packet[DATA_START_INDEX + 3] = quint8(data & 0x000000FF);
    m_packet+=CalculateChecksum(m_packet.size()-1);
}
Frame::Frame(quint8 cmd, qint32 data, QObject *parent ): QObject(parent)
{
    m_packet[FRAME_START_INDEX] = FRAME_PREAMBLE_A;
    m_packet[FRAME_PREAMBLE_2_INDEX] = FRAME_PREAMBLE_B;
    m_packet[CMD_CODE_INDEX] = cmd;
    m_packet[DATA_LEN_INDEX] = 4;
    m_packet[DATA_START_INDEX] = quint8((quint32(data) & 0xFF000000) >> 24);
    m_packet[DATA_START_INDEX + 1] = quint8((data & 0x00FF0000) >> 16);
    m_packet[DATA_START_INDEX + 2] = quint8((data & 0x0000FF00) >> 8);
    m_packet[DATA_START_INDEX + 3] = quint8(data & 0x000000FF);
    m_packet+=CalculateChecksum(m_packet.size()-1);

}
Frame::Frame(quint8 cmd, QByteArray data, QObject *parent ): QObject(parent)
{
    m_packet[FRAME_START_INDEX] = FRAME_PREAMBLE_A;
    m_packet[FRAME_PREAMBLE_2_INDEX] = FRAME_PREAMBLE_B;
    m_packet[CMD_CODE_INDEX] = cmd;
    m_packet[DATA_LEN_INDEX] = quint8(data.count());
    for(int i = 0; i < data.count(); i++)
    {
       m_packet[DATA_START_INDEX + i] = quint8(data[i]);
    }
    m_packet+=CalculateChecksum(m_packet.size()-1);
}

Frame::~Frame()
{
    m_packet.clear();
}

quint8 Frame::GetCmd()
{
    quint8 cmd = 0;
    if(m_packet.count() > 4)
    {
        cmd = quint8(m_packet[CMD_CODE_INDEX]);
    }
    return cmd;
}

quint8 Frame::GetDataLength()
{
    quint8 data_len = 0;
    if(m_packet.count() > CMD_CODE_INDEX)
    {
        data_len = quint8(m_packet[DATA_LEN_INDEX]);
    }
    return data_len;
}

quint8 Frame::GetUByte()
{
    quint8 ret_val = 0;
    if(this->GetDataLength() > 0)
    {
        ret_val = quint8(m_packet[DATA_LEN_INDEX]);
    }
    return ret_val;
}

qint8 Frame::GetSByte()
{
    qint8 ret_val = 0;
    if(this->GetDataLength() > 0)
    {
        ret_val = qint8(m_packet[DATA_LEN_INDEX]);
    }
    return ret_val;

}

quint8 Frame::GetUByte(int index)
{
    quint8 ret_val = 0;
    if(this->GetDataLength() > 0)
    {
        ret_val = quint8(m_packet[DATA_LEN_INDEX + index]);
    }
    return ret_val;
}
quint16 Frame::GetUInt16()
{
    quint16 ret_val = 0;
    if(this->GetDataLength() > 0)
    {
        ret_val = quint16(MakeWord(m_packet[DATA_LEN_INDEX + 1], m_packet[DATA_LEN_INDEX]));
    }
    return ret_val;
}

quint16 Frame::GetUInt16(int index)
{
    quint16 ret_val = 0;
    if(this->GetDataLength() > 0)
    {
        ret_val = quint16(MakeWord(m_packet[DATA_LEN_INDEX + 1 + index], m_packet[DATA_LEN_INDEX + index]));
    }
    return ret_val;
}
qint16 Frame::GetInt16()
{
    qint16 ret_val = 0;
    if(this->GetDataLength() > 0)
    {
        ret_val = qint16(MakeWord(m_packet[DATA_LEN_INDEX + 1], m_packet[DATA_LEN_INDEX]));
    }
    return ret_val;
}
quint32 Frame::GetUInt32()
{
   quint32 ret_val = 0;
   if(this->GetDataLength() > 0)
   {
       ret_val = quint32(MakeDWord(MakeWord(m_packet[DATA_LEN_INDEX + 3], m_packet[DATA_LEN_INDEX + 2]),
                         MakeWord(m_packet[DATA_LEN_INDEX + 1], m_packet[DATA_LEN_INDEX])));
   }
   return ret_val;

}
quint32 Frame::GetUInt32(int index)
{
    quint32 ret_val = 0;
    if(this->GetDataLength() > 0)
    {
        ret_val = quint32(MakeDWord(MakeWord(m_packet[DATA_LEN_INDEX + 3 + index], m_packet[DATA_LEN_INDEX + 2 + index]),
                          MakeWord(m_packet[DATA_LEN_INDEX + 1 + index], m_packet[DATA_LEN_INDEX + index])));
    }
    return ret_val;
}
qint32 Frame::GetInt32()
{
    qint32 ret_val = 0;
    if(this->GetDataLength() > 0)
    {
        ret_val = qint32(MakeDWord(MakeWord(m_packet[DATA_LEN_INDEX + 3], m_packet[DATA_LEN_INDEX + 2]),
                          MakeWord(m_packet[DATA_LEN_INDEX + 1], m_packet[DATA_LEN_INDEX])));
    }
    return ret_val;
}

void Frame::Clear()
{
    m_packet.clear();
}

void Frame::AddByte(quint8 data)
{
    m_packet.append(data);
}

QByteArray Frame::GetBuffer()
{
    return m_packet;
}

quint8 Frame::CalculateChecksum(size_t len)
{
    quint32 i = 0;
    quint8 crc8 = 0;
    for (i = 0; i < len; i++)
    {
        crc8 = crc_table[crc8 ^ m_packet[i]];
    }
    return crc8;
}
