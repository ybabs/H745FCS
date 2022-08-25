#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMutex>
#include <QQueue>
#include "inc/frame.h"


class SerialManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialManager( QQueue<Frame*> *frameQueue, QObject * parent = nullptr);
    ~SerialManager();

    QString GetPortName() const;
    void SetPortName(const QString &value);
    void RequestWork();
    void Abort();
    void SetupSerial();


private:

    bool abort;
    bool in_use;
    QMutex mutex;
    QSerialPort *m_serial;
    QQueue<Frame*> *m_frame_queue;

    void SendData(Frame * frame);

signals:
    void workRequested();
    void finished();
    void frameReceived(Frame *frame);
    void valueChanged(const QString &value);


public slots:
    void stateMachine();
    void sendUint8(quint8 cmd, quint8 data);
    void sendUint16(quint8 cmd, quint16 data);
    void sendUint32(quint8 cmd, quint32 data);
    void sendFrame(Frame *frame);

};

#endif // SERIALMANAGER_H
