#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class SerialManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialManager(QObject * parent = nullptr);
    ~SerialManager();

    QString getPortName() const;
    void setPortName(const QString &value);


signals:



public slots:

private:

};

#endif // SERIALMANAGER_H
