#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

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

protected:

private slots:
    void open();
    void parsePath();
    void uploadPath();
    void comSettings();
    void dimensionsSettings();


private:
    void createActions();
    void createMenus();

    bool saveFile(const QByteArray &fileFormat);


    QWidget* mWindow;
    ScribbleArea* paintArea;
    SerialCom* serialCom;
    PathPlanner* pathPlaner;
    Ui::MainWindow *ui;

    // The menu widgets
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;

    // All the actions that can occur
    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *printAct;
    QAction *parsePathAct;
    QAction *uploadPathAct;
    QAction *clearScreenAct;
    QAction *comSettingsAct;
    QAction *dimSettingsAct;
};
#endif
