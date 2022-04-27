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
    portNumber=-1;
    boudRate=-1;
}

SerialCom::~SerialCom()
{
    serialPort->close();
}

/**
 * @brief SerialCom::write function sending message on serial port
 * @param text
 */
void SerialCom::write(const char* text)
{
    serialPort->write(text);
}

/**
 * @brief SerialCom::setComParams function setting params for serial communication
 * @param port
 * @param boudRate
 */
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

/**
 * @brief SerialCom::createDataFrame function creating frame from path data and sending if possible
 */
void SerialCom::createDataFrame()
{
    char buffDistance[4]="";
    char buffRotation[4]="";
    int tempVal;
    int size[2];
    char dataPack[16];
    memset(dataBuffer,0,300);
    if(pathData.getStepDataSize()<16)
    {
        sprintf(dataBuffer,"0%X",pathData.getStepDataSize());
    }
    else
    {
        sprintf(dataBuffer,"%X",pathData.getStepDataSize());
    }
    for(int i=0;i<pathData.getStepDataSize();i++)
    {
        //clearing buffer for next step
        memset(dataPack,0,16);
        //paste distance and rotation
        tempVal=pathData.getStepData(i).distance*100;
        size[0]=sprintf(buffDistance,"%X",tempVal);
        tempVal=pathData.getStepData(i).rotation*100;
        size[1]=sprintf(buffRotation,"%X",tempVal);

        qDebug()<<buffDistance<<" "<<buffRotation;

        if(i<16)
        {
            sprintf(dataPack,"0%X",i);
        }
        else
        {
            sprintf(dataPack,"%X",i);
        }
        dataPack[2]=size[0]+'0';
        dataPack[3]=size[1]+'0';
        strcat_s(dataPack,buffDistance);
        strcat_s(dataPack,buffRotation);
        strcat_s(dataBuffer,dataPack);
        qDebug()<<dataBuffer;

    }

    if(serialPort->isOpen())
    {
        serialPort->write(dataBuffer,strlen(dataBuffer));
    }
    memset(dataBuffer,0,300);
}

int SerialCom::getBoudRate()
{
    return boudRate;
}

int SerialCom::getSerialPort()
{
    return portNumber;
}


