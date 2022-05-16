#include "pathplaner.h"
#include "dimensiondata.h"

#include "imageinterpreter.h"
#include<QDebug>
PathPlaner::PathPlaner()
{
    dimensions = new DimensionData;
    imageParser = new ImageInterpreter(*this);
}

bool PathPlaner::processPath(QImage& img)
{
    imageParser->setImage(img);
    imageParser->processImage();
    return false;
}

void PathPlaner::setDimensions(float realH, float realW, int imgH, int imgW)
{
    dimensions->setImageDim(imgH,imgW);
    dimensions->setRealDim(realH,realW);
    Scales=dimensions->getScaleFactor();
    qDebug()<<Scales.first<<" "<<Scales.second;
}

void PathPlaner::setStartingPos(QPoint pos)
{
    startingPos=pos;
}

void PathPlaner::addStep(QPoint step)
{
    pathSteps.push_back(step);
}

QPoint PathPlaner::getStartingPos()
{
    return startingPos;
}
