#include "ui_mainwindow.h"
#include <QtWidgets>

#include "mainwindow.h"
#include "scribblearea.h"
#include "serialcom.h"
#include "comsettingsview.h"
#include "dimensionsinput.h"
#include "pathplanner.h"
#include "dimensiondata.h"
//#include "menuBar.h"
// MainWindow constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mWindow=new QWidget();
    pathPlaner=new PathPlanner;
    serialCom= new SerialCom(*pathPlaner);
    paintArea=new ScribbleArea;
    pathPlaner->getDimensions()->setRealDim(3,3);

    //create menu bar
    createActions();
    createMenus();

    //setup window layout
    mWindow->setLayout(ui->gridLayout);
    setCentralWidget(mWindow);
    ui->gridLayout->addWidget(paintArea,0,0,1,4);
    setWindowTitle(tr("PathPlanner"));
    this->setWindowState(Qt::WindowMaximized);

    //connects events
    connect(paintArea,SIGNAL(pointAdded(QPoint)),pathPlaner,SLOT(AddStep(QPoint)));
}


// Check if the current image has been changed and then
// open a dialog to open a file
void MainWindow::open()
{

    // Get the file to open from a dialog
    // tr sets the window title to Open File
    // QDir opens the current dirctory
    QString fileName = QFileDialog::getOpenFileName(this,
                               tr("Open Image "), QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp)"));

    // If we have a file name load the image and place
    // it in the paintArea
    if (!fileName.isEmpty())
    {
        paintArea->openImage(fileName);
    }

    pathPlaner->getDimensions()->setImageDim(paintArea->getOriginalDimensions().first,paintArea->getOriginalDimensions().second);

}

//start parsing a path
void MainWindow::parsePath()
{
    pathPlaner->parsePath();
}

//upload path to robot
void MainWindow::uploadPath()
{
    serialCom->createDataFrame();
}

//open a dialog to change transmission settings
void MainWindow::comSettings()
{
    ComSettingsView settingsView(*serialCom);
    settingsView.setModal(true);
    settingsView.exec();
    serialCom->write("test");
    ui->label_boud->setText(QString("Boud Rate:%1").arg(serialCom->getBoudRate()));
    ui->label_com->setText(QString("COM:%1").arg(serialCom->getSerialPort()));
}

//open a dialog to change dimensions settings
void MainWindow::dimensionsSettings()
{
    DimensionsInput dimensionsView(*pathPlaner->getDimensions());
    dimensionsView.setModal(true);
    dimensionsView.exec();
    ui->label_dim->setText(QString("Dimensions:%1x%2")
                           .arg(pathPlaner->getDimensions()->getRealDimensions().first)
                           .arg(pathPlaner->getDimensions()->getRealDimensions().second));

}

// Define menu actions that call functions
void MainWindow::createActions()
{
    // Create the action tied to the menu
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));


    parsePathAct = new QAction(tr("&Parse Path"),this);
    parsePathAct->setShortcut(tr("Ctrl+B"));
    connect(parsePathAct, SIGNAL(triggered()), this, SLOT(parsePath()));

    uploadPathAct = new QAction(tr("&Upload Path"),this);
    uploadPathAct->setShortcut(tr("Ctrl+R"));
    connect(uploadPathAct, SIGNAL(triggered()), this, SLOT(uploadPath()));

    // Create exit action and tie to MainWindow::close()
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));


    // Create clear screen action and tie to MainWindow::clearImage()
    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()),paintArea, SLOT(clearImage()));

    comSettingsAct = new QAction(tr("&COM Settings"),this);
    comSettingsAct->setShortcut(tr("Crtl+T"));
    connect(comSettingsAct, SIGNAL(triggered()), this, SLOT(comSettings()));

    dimSettingsAct = new QAction(tr("&Dimensions"),this);
    dimSettingsAct->setShortcut(tr("Crtl+D"));
    connect(dimSettingsAct, SIGNAL(triggered()), this, SLOT(dimensionsSettings()));


}

// Create the menubar
void MainWindow::createMenus()
{
    // Create Save As option and the list of file types
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach (QAction *action, saveAsActs)
        saveAsMenu->addAction(action);

    // Attach all actions to File
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(parsePathAct);
    fileMenu->addAction(uploadPathAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    // Attach all actions to Options
    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(clearScreenAct);
    optionMenu->addAction(comSettingsAct);
    optionMenu->addAction(dimSettingsAct);

    // Add menu items to the menubar
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);

}






