#include "menuBar.h"
#include "scribblearea.h"
#include "pathplanner.h"
#include "serialcom.h"
#include "comsettingsview.h"
#include "dimensionsinput.h"
#include "mainwindow.h"

#include <QFileDialog>
#include <QMenuBar>

MenuBar::MenuBar(MainWindow& window):
    mainWindow(window)
{

}

MenuBar::~MenuBar()
{

}

void MenuBar::connectActions()
{
    ScribbleArea &area = mainWindow.getPainter();
    createActions();
    createMenus();
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    connect(parsePathAct, SIGNAL(triggered()), this, SLOT(parsePath()));
    connect(uploadPathAct, SIGNAL(triggered()), this, SLOT(uploadPath()));
    connect(clearScreenAct, SIGNAL(triggered()), &area, SLOT(clearImage()));
    connect(comSettingsAct, SIGNAL(triggered()), this, SLOT(comSettings()));
    connect(dimSettingsAct, SIGNAL(triggered()), this, SLOT(dimensionsSettings()));
}

void MenuBar::open()
{
    PathPlanner &pathPlaner = mainWindow.getPlanner();
    ScribbleArea &paintArea = mainWindow.getPainter();
    // Get the file to open from a dialog
    // tr sets the window title to Open File
    // QDir opens the current dirctory
    QString fileName = QFileDialog::getOpenFileName(&mainWindow,
                               tr("Open Image "), QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp)"));

    // If we have a file name load the image and place
    // it in the paintArea
    if (!fileName.isEmpty())
    {
        paintArea.openImage(fileName);
    }

    pathPlaner.getDimensions()->setImageDim(paintArea.getOriginalDimensions().first,paintArea.getOriginalDimensions().second);
}

void MenuBar::parsePath()
{
    mainWindow.getPlanner().parsePath();
}

void MenuBar::uploadPath()
{
    mainWindow.getCom().createDataFrame();
}

void MenuBar::comSettings()
{
    ComSettingsView settingsView(mainWindow.getCom());
    settingsView.setModal(true);
    settingsView.exec();
    emit labelNeedChange();

}

void MenuBar::dimensionsSettings()
{
    DimensionsInput dimensionsView(*mainWindow.getPlanner().getDimensions());
    dimensionsView.setModal(true);
    dimensionsView.exec();
    emit labelNeedChange();

}

// Define menu actions that call functions
void MenuBar::createActions()
{
    // Create the action tied to the menu
    openAct = new QAction(tr("&Open..."), &mainWindow);
    openAct->setShortcuts(QKeySequence::Open);

    parsePathAct = new QAction(tr("&Parse Path"),&mainWindow);
    parsePathAct->setShortcut(tr("Ctrl+B"));

    uploadPathAct = new QAction(tr("&Upload Path"),&mainWindow);
    uploadPathAct->setShortcut(tr("Ctrl+R"));

    // Create exit action and tie to MainWindow::close()
    exitAct = new QAction(tr("E&xit"), &mainWindow);
    exitAct->setShortcuts(QKeySequence::Quit);

    // Create clear screen action and tie to MainWindow::clearImage()
    clearScreenAct = new QAction(tr("&Clear Screen"), &mainWindow);
    clearScreenAct->setShortcut(tr("Ctrl+L"));

    comSettingsAct = new QAction(tr("&COM Settings"),&mainWindow);
    comSettingsAct->setShortcut(tr("Crtl+T"));

    dimSettingsAct = new QAction(tr("&Dimensions"),&mainWindow);
    dimSettingsAct->setShortcut(tr("Crtl+D"));



}

// Create the menubar
void MenuBar::createMenus()
{
    // Create Save As option and the list of file types
    saveAsMenu = new QMenu(tr("&Save As"), &mainWindow);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);

    // Attach all actions to File
    fileMenu = new QMenu(tr("&File"), &mainWindow);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(parsePathAct);
    fileMenu->addAction(uploadPathAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    // Attach all actions to Options
    optionMenu = new QMenu(tr("&Options"), &mainWindow);
    optionMenu->addAction(clearScreenAct);
    optionMenu->addAction(comSettingsAct);
    optionMenu->addAction(dimSettingsAct);

    // Add menu items to the menubar
    mainWindow.menuBar()->addMenu(fileMenu);
    mainWindow.menuBar()->addMenu(optionMenu);

}