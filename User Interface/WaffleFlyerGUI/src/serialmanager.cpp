#include <QTimer>
#include <QThread>
#include <QEventLoop>
#include <QDebug>

#include "inc/uartconfig.h"
#include "inc/serialmanager.h"



SerialManager::SerialManager( QQueue<Frame*> *frameQueue, QObject * parent)
{
   in_use = false;
   abort = false;
   m_serial = nullptr;
   m_frame_queue = frameQueue;
}

SerialManager::~SerialManager()
{
    if(m_serial != nullptr)
    {
        delete m_serial;
    }
}


void SerialManager::Abort()
{
    mutex.lock();
    if(in_use)
    {
      abort = true;
      qDebug() << "Request aborting in thread " << thread()->currentThreadId();
    }
    mutex.unlock();
}

void SerialManager::RequestWork()
{
    mutex.lock();
    in_use = true;
    abort = false;
    qDebug() << "Request start in thread " << thread()->currentThreadId();
    mutex.unlock();
   emit workRequested();
}

void SerialManager::SetupSerial()
{
    m_serial = new QSerialPort();
    m_serial->setPortName("COM5");
    m_serial->setBaudRate(QSerialPort::Baud115200);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    m_serial->open(QIODevice::ReadWrite);
    qDebug() << "SerialPort Status: " << m_serial->isOpen();

}

void SerialManager::stateMachine()
{
    qDebug()<<"Starting State Machine process in Thread "<<thread()->currentThreadId();
    bool m_abort = false;
    UartFSM uart_fsm {UartFSM::WAIT_PREAMBLE_1};

    quint8 input_byte;

    Frame *m_inputFrame = nullptr;
    quint8 data_len = 0;
    quint_8 crc8  = 0;

    while(!m_abort)
    {
        mutex.lock();
        m_abort = abort;
        mutex.unlock();

        if(!m_frame_queue->isEmpty())
        {
            Frame *out_frame = m_frame_queue->dequeue();
            sendFrame(out_frame);
            delete out_frame;
        }
        else
        {
            if(m_serial->waitForReadyRead(1))
            {
                QByteArray rx_data = m_serial->readAll();

                while(rx_data.count() > 0)
                {
                    input_byte = quint8(rx_data[0]);
                    rx_data.remove(0, 1);

                    switch(uart_fsm)
                    {

                    }

                }
            }
        }

    }

}

void SerialManager::sendFrame(Frame* frame)
{
    if(m_serial != nullptr && m_serial->isOpen() && frame != nullptr)
    {
        SendData(frame);
    }
}

void SerialManager::sendUint8(quint8 cmd, quint8 data)
{
    if(m_serial != nullptr && m_serial->isOpen())
    {
        Frame frameToSend(cmd, data);
        SendData(&frameToSend);
    }
}
void SerialManager::sendUint16(quint8 cmd, quint16 data)
{
    if(m_serial != nullptr && m_serial->isOpen())
        {
            Frame frameToSend(cmd, data);
            SendData(&frameToSend);
        }
}
void SerialManager::sendUint32(quint8 cmd, quint32 data)
{
    if(m_serial != nullptr && m_serial->isOpen())
    {
        Frame frameToSend(cmd, data);
        SendData(&frameToSend);
    }
}

void SerialManager::SendData(Frame * frame)
{
    QByteArray out_buffer;
    int data_to_send = 0;
    int frame_length  = frame->GetDataLength();

    QByteArray frame_buffer = frame->GetBuffer();
    out_buffer[data_to_send++] = Frame::FRAME_PREAMBLE_A;

    quint8 value;

    for(int i = 1; i < frame_length; i++)
    {
        value = frame_buffer[i];
        if(value == Frame::FRAME_PREAMBLE_A)
        {
            out_buffer[data_to_send++] = Frame::FRAME_PREAMBLE_B;
        }

        else
        {
            out_buffer[data_to_send++] = value;
        }
    }
    //TODO  compute checksum and append;
    m_serial->write(out_buffer);
}
