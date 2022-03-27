#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

class MenuBar;
class ScribbleArea;
class SerialCom;
class PathPlanner;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    SerialCom& getCom();
    PathPlanner& getPlanner();
    ScribbleArea& getPainter();

private slots:
    void updateLabels();


private:

    bool saveFile(const QByteArray &fileFormat);

    MenuBar* menu;
    QWidget* mWindow;
    ScribbleArea* paintArea;
    SerialCom* serialCom;
    PathPlanner* pathPlaner;
    Ui::MainWindow *ui;

};
#endif
