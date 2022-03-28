#include "ui_mainwindow.h"
#include <QtWidgets>

#include "mainwindow.h"
#include "scribblearea.h"
#include "serialcom.h"
#include "comsettingsview.h"
#include "dimensionsinput.h"
#include "pathplanner.h"
#include "dimensiondata.h"
#include "menuBar.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mWindow = new QWidget();
    pathPlaner = new PathPlanner;
    serialCom = new SerialCom(*pathPlaner);
    paintArea = new ScribbleArea;
    menu= new MenuBar(*this);

    menu->connectActions();


    //setup window layout
    mWindow->setLayout(ui->gridLayout);
    setCentralWidget(mWindow);
    ui->gridLayout->addWidget(paintArea,0,0,1,4);
    setWindowTitle(tr("PathPlanner"));
    this->setWindowState(Qt::WindowMaximized);
    updateLabels();
    //connects events
    connect(paintArea,SIGNAL(pointAdded(QPoint)),pathPlaner,SLOT(AddStep(QPoint)));
    connect(menu,SIGNAL(labelNeedChange()),this,SLOT(updateLabels()));
}



SerialCom& MainWindow::getCom()
{
    return *serialCom;
}

PathPlanner& MainWindow::getPlanner()
{
    return *pathPlaner;
}

ScribbleArea& MainWindow::getPainter()
{
    return *paintArea;
}

void MainWindow::updateLabels()
{
    //transmission labels
    ui->label_boud->setText(QString("Boud Rate:%1").arg(serialCom->getBoudRate()));
    ui->label_com->setText(QString("COM:%1").arg(serialCom->getSerialPort()));

    //dimensions label
    ui->label_dim->setText(QString("Dimensions:%1x%2")
                           .arg(pathPlaner->getDimensions()->getRealDimensions().first)
                           .arg(pathPlaner->getDimensions()->getRealDimensions().second));
}
