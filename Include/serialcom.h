#ifndef SERIALCOM_H
#define SERIALCOM_H
#include<iostream>
class QSerialPort;
class QString;
class PathPlanner;

class SerialCom
{
public:
    SerialCom(PathPlanner& pathPlaner);
    virtual ~SerialCom();
    void write(const char* text);
    void setComParams(int port,int boudRate);
    void uploadData();
    void createDataFrame();
    int getBoudRate();
    int getSerialPort();
private:
    int portNumber;
    int boudRate;
    char dataBuffer[300];
    PathPlanner& pathData;
    QSerialPort* serialPort;

};

#endif // SERIALCOM_H
