#include "serialcom.h"

#include<QSerialPort>
#include<QString>
#include<string.h>
#include<QDebug>
#include"pathplanner.h"


SerialCom::SerialCom(PathPlanner& pathPlaner):
    pathData(pathPlaner)
{
    serialPort=new QSerialPort;
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->setStopBits(QSerialPort::OneStop);

}

SerialCom::~SerialCom()
{
    serialPort->close();
}

void SerialCom::write(const char* text)
{
    serialPort->write(text);
}

void SerialCom::setComParams(int port, int boudRate)
{
    portNumber=port;
    this->boudRate=boudRate;
    QString portName = QString("com%1").arg(port);
    serialPort->close();
    serialPort->setPortName(portName);
    serialPort->setBaudRate(boudRate);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->open(QIODevice::ReadWrite);
}

void SerialCom::createDataFrame()
{
    char buffDistance[4]="";
    char buffRotation[4]="";
    int tempVal;
    int size;
    char dataPack[16];
    for(int i=0;i<pathData.getStepDataSize();i++)
    {
        memset(dataPack,0,16);
        tempVal=pathData.getStepData(i).distance*100;
        size=sprintf(buffDistance,"%X",tempVal);
        tempVal=pathData.getStepData(i).rotation*100;
        size+=sprintf(buffRotation,"%X",tempVal);
        qDebug()<<buffDistance<<" "<<buffRotation;

        if(i<16)
        {
            sprintf(dataPack,"0%X",i);
        }
        else
        {
            sprintf(dataPack,"%X",i);
        }
        dataPack[2]='0';
        dataPack[3]=size+'0';
        strcat_s(dataPack,buffDistance);
        strcat_s(dataPack,buffRotation);
        strcat_s(dataBuffer,dataPack);
        qDebug()<<dataBuffer;

    }

    if(serialPort->isOpen())
    {
        serialPort->write(dataBuffer,strlen(dataBuffer));
    }
}

int SerialCom::getBoudRate()
{
    return boudRate;
}

int SerialCom::getSerialPort()
{
    return portNumber;
}


