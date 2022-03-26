#ifndef DIMENSIONSINPUT_H
#define DIMENSIONSINPUT_H

#include <QDialog>
#include <QPair>
namespace Ui {
class DimensionsInput;
}

class DimensionData;

class DimensionsInput : public QDialog
{
    Q_OBJECT

public:
    explicit DimensionsInput(DimensionData& data,QWidget *parent = nullptr);
    ~DimensionsInput();

private slots:
    void on_OkButton_clicked();
    void on_CancelButton_clicked();

private:
    DimensionData& dimData;
    Ui::DimensionsInput *ui;
};

#endif // DIMENSIONSINPUT_H
