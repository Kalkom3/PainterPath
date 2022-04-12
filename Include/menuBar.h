#pragma once

#include <QObject>
#include <QMenu>

enum ESetupProgress{
    ImageOpened = 0x01,
    ComSet = 0x02,
    BounRateSet = 0x04,
    DimensionsSet = 0x08,
    PathParsed = 0x10
};

enum EErrorType
{
    setup,
    collision
};

class MainWindow;
class ImageInterpreter;

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
    void showErrorMessage(EErrorType type);
    uint16_t setupProgress;//checking state of following setup steps: set image, set com, set boudRate, set dimensions, parse path

    ImageInterpreter* imgInterpreter;
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
