#include "dimensionsinput.h"
#include "ui_dimensionsinput.h"
#include "dimensiondata.h"
DimensionsInput::DimensionsInput(DimensionData& data,QWidget *parent) :
    QDialog(parent),
    dimData(data),
    ui(new Ui::DimensionsInput)
{
    ui->setupUi(this);
    setWindowTitle(tr("Dimensions settings"));

}


void DimensionsInput::on_OkButton_clicked()
{
    dimData.setRealDim(ui->HeightBox->value(),ui->WidthBox->value());
    qDebug()<<ui->HeightBox->value()<<" "<<ui->WidthBox->value();
    this->close();
}


void DimensionsInput::on_CancelButton_clicked()
{
    this->close();
}


DimensionsInput::~DimensionsInput()
{
    delete ui;
}
