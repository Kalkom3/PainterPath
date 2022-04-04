#include "menuBar.h"
#include "scribblearea.h"
#include "pathplanner.h"
#include "serialcom.h"
#include "comsettingsview.h"
#include "dimensionsinput.h"
#include "mainwindow.h"

#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QStandardPaths>

MenuBar::MenuBar(MainWindow& window):
    mainWindow(window)
{
    setupProgress=0;
}

MenuBar::~MenuBar()
{

}

void MenuBar::connectActions()
{
    createActions();
    createMenus();
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    connect(parsePathAct, SIGNAL(triggered()), this, SLOT(parsePath()));
    connect(uploadPathAct, SIGNAL(triggered()), this, SLOT(uploadPath()));
    connect(clearScreenAct, SIGNAL(triggered()), &mainWindow.getPainter(), SLOT(clearImage()));
    connect(clearScreenAct, SIGNAL(triggered()), &mainWindow.getPlanner(), SLOT(clearPathPoints()));
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
    QString fileName = QFileDialog::getOpenFileName(&mainWindow,tr("Open Image "), 
                               QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                               tr("Image Files (*.png *.jpg *.bmp)"));

    // If we have a file name load the image and place
    // it in the paintArea
    if (!fileName.isEmpty())
    {
        paintArea.openImage(fileName);
        setupProgress |= ESetupProgress::ImageOpened;
    }

    pathPlaner.getDimensions()->setImageDim(paintArea.getOriginalDimensions().first,paintArea.getOriginalDimensions().second);
}

void MenuBar::parsePath()
{
    if(setupProgress >=15)
    {
        mainWindow.getPlanner().parsePath();
        setupProgress |= ESetupProgress::PathParsed;
    }
    else
    {
        showSetupErrorMessage();
    }
}

void MenuBar::uploadPath()
{
    if(setupProgress >= 31)
    {
        mainWindow.getCom().createDataFrame();
    }
    else if (setupProgress >= 15)
    {
        mainWindow.getPlanner().parsePath();
        setupProgress |= ESetupProgress::PathParsed;
        mainWindow.getCom().createDataFrame();
    }
    else
    {
        showSetupErrorMessage();
    }

}

void MenuBar::comSettings()
{
    ComSettingsView settingsView(mainWindow.getCom());
    settingsView.setModal(true);
    settingsView.exec();
    emit labelNeedChange();
    if(mainWindow.getCom().getBoudRate()>0)
    {
        setupProgress |= ESetupProgress::BounRateSet;
    }
    if(mainWindow.getCom().getSerialPort()>0)
    {
        setupProgress |= ESetupProgress::ComSet;
    }
}

void MenuBar::dimensionsSettings()
{
    DimensionsInput dimensionsView(*mainWindow.getPlanner().getDimensions());
    dimensionsView.setModal(true);
    dimensionsView.exec();
    emit labelNeedChange();
    if(mainWindow.getPlanner().getDimensions()->getRealDimensions().first>0
            && mainWindow.getPlanner().getDimensions()->getRealDimensions().second>0)
    {
        setupProgress |= ESetupProgress::DimensionsSet;
    }

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
    fileMenu->addAction(clearScreenAct);
    fileMenu->addSeparator();
    fileMenu->addAction(parsePathAct);
    fileMenu->addAction(uploadPathAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    // Attach all actions to Options
    optionMenu = new QMenu(tr("&Options"), &mainWindow);
    optionMenu->addAction(comSettingsAct);
    optionMenu->addAction(dimSettingsAct);

    // Add menu items to the menubar
    mainWindow.menuBar()->addMenu(fileMenu);
    mainWindow.menuBar()->addMenu(optionMenu);

}

void MenuBar::showSetupErrorMessage()
{
    QMessageBox errorBox;
    errorBox.setText("Program setup is not compleated!");
    QString message;
    if(!(setupProgress & ESetupProgress::ImageOpened))
    {
        message+="-Open image\n";
    }
    if(!(setupProgress & ESetupProgress::ComSet))
    {
        message+="-Set transminnion port\n";
    }
    if(!(setupProgress & ESetupProgress::BounRateSet))
    {
        message+="-Set boudRate\n";
    }
    if(!(setupProgress & ESetupProgress::DimensionsSet))
    {
        message+="-Set dimensions\n";
    }
    errorBox.setInformativeText(message);
    errorBox.setIcon(QMessageBox::Critical);
    errorBox.exec();

}


