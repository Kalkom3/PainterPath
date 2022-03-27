#pragma once

#include <QObject>
#include <QMenu>

class ScribbleArea;
class SerialCom;
class PathPlanner;
class MainWindow;

class MenuBar : public QObject
{
    Q_OBJECT
public:
    MenuBar(MainWindow& window);
    ~MenuBar();
    void connectActions();

signals:
    void labelNeedChange();

private slots:
    void open();
    void parsePath();
    void uploadPath();
    void comSettings();
    void dimensionsSettings();


private:
    void createActions();
    void createMenus();

    MainWindow& mainWindow;
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
