#include "comsettingsview.h"
#include "ui_comsettingsview.h"
#include "serialcom.h"
#include<QtSerialPort>


ComSettingsView::ComSettingsView(SerialCom &com, QWidget *parent):
    QDialog(parent),
    ui(new Ui::ComSettingsView),
    com(com)
{
    ui->setupUi(this);
    setWindowTitle(tr("COM settings"));
    ui->boudBox->setCurrentText(QString::number(com.getSerialPort()));
    ui->boudBox->addItem("4800");
    ui->boudBox->addItem("9600");
    ui->boudBox->addItem("38400");
    ui->boudBox->addItem("57600");
    ui->boudBox->addItem("115200");
    QList<QSerialPortInfo>portList=QSerialPortInfo::availablePorts();
    for(QSerialPortInfo& portInfo : portList)
    {
        ui->portBox->addItem(portInfo.portName());
    }
}

ComSettingsView::~ComSettingsView()
{
    delete ui;
}

void ComSettingsView::on_okButton_clicked()
{
    QString port = ui->portBox->currentText();
    int portNr=port.right(1).toInt();
    int boudRate = ui->boudBox->currentText().toInt();
    com.setComParams(portNr,boudRate);
    this->close();
}


void ComSettingsView::on_cancelButton_clicked()
{
    this->close();
}

