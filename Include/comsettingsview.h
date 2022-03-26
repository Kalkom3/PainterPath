#ifndef COMSETTINGSVIEW_H
#define COMSETTINGSVIEW_H

#include <QDialog>

namespace Ui {
class ComSettingsView;
}

class SerialCom;

class ComSettingsView : public QDialog
{
    Q_OBJECT

public:
    explicit ComSettingsView(SerialCom& com,QWidget *parent = nullptr);
    ~ComSettingsView();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ComSettingsView *ui;
    SerialCom& com;
};

#endif // COMSETTINGSVIEW_H
