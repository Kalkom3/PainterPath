#include "imageinterpreter.h"

#include<pathplaner.h>
#include<QDebug>

ImageInterpreter::ImageInterpreter(PathPlaner& pathPlaner):
    path(pathPlaner)
{
}

void ImageInterpreter::setImage(QImage &img)
{
    image=&img;
}

void ImageInterpreter::processImage()
{
    int currentDestination;
    qDebug()<<path.getStartingPos();
    QPoint destinationPoint;
    //check first position
    if(image->pixelColor(path.getStartingPos())==Qt::green)
    {
        qDebug()<<"git";
    }
    qDebug()<<findFirstStep();
    destinationPoint=findFirstStep();
    currentDestination=findDestination(path.getStartingPos(),destinationPoint);
    qDebug()<<"Cd"<<currentDestination;
    checkFrameStatus();
}

bool ImageInterpreter::checkBorders(QPoint point)
{
    return (point.x()<=0 || point.x()>=image->width() || point.y()<=0 || point.y()>=image->height());
}

int ImageInterpreter::findDestination(QPoint startPoint, QPoint nextPoint)
{
    int xDest = startPoint.x()-nextPoint.x();
    int yDest = startPoint.y()-nextPoint.y();
    if(abs(xDest)>abs(yDest))
    {
        pixFrame.orientation=Eorientation::horizontal;
        for(int i=0;i<PIXFRAME_SIZE;i++)
        {
            pixFrame.pixels[i]=QPoint(nextPoint.x(),nextPoint.y()+(i-1));
        }
        if(xDest>0)
        {
            return 4;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        pixFrame.orientation=Eorientation::vertical;
        for(int i=0;i<PIXFRAME_SIZE;i++)
        {
            pixFrame.pixels[i]=QPoint(nextPoint.x()+(i-1),nextPoint.y());
        }
        if(yDest>0)
        {
            return 1;
        }
        else
        {
            return 3;
        }
    }


}

QPoint ImageInterpreter::findFirstStep()
{
    int distance=12;
    bool roundDone=true;
    QPoint stepPoint=QPoint(0,0);
    QPoint checkingPoint=QPoint(path.getStartingPos().x(),path.getStartingPos().y());
    while(distance<32)
    {
        if(roundDone)
        {
            distance++;
            checkingPoint.setX(path.getStartingPos().x()-distance);
            checkingPoint.setY(path.getStartingPos().y()-distance);
            roundDone=false;
        }

        for(int i=0;i<(1+distance*2);i++)
        {
            for(int j=0;j<(1+distance*2);j++)
            {
                checkingPoint.setX(checkingPoint.x()+1);
                if((i>0 && i<(distance*2)) && (j>0 && j<(distance*2)))
                {
                    continue;
                }
                if(image->pixelColor(checkingPoint)==Qt::red)
                {
                    stepPoint=checkingPoint;
                    i=(1+distance*2);
                    break;
                }
            }
            checkingPoint.setX(path.getStartingPos().x()-distance);
            checkingPoint.setY(checkingPoint.y()+1);
        }
        if(stepPoint.x()>0)
        {
            break;
        }
        roundDone=true;

    }
    return stepPoint;
}

/**
 * @brief Function check current colors of pixels in tested frame
 */
void ImageInterpreter::checkFrameStatus()
{
    QString ss;
    int redPoints=0;
    pixFrame.botIndex=0;
    pixFrame.topIndex=0;
    for(int i=0;i<PIXFRAME_SIZE;i++)
    {
        if(image->pixelColor(pixFrame.pixels[i])==Qt::red)
        {
            ss+='r';
            if(redPoints==0)
            {
                pixFrame.topIndex=i;
            }
            redPoints++;
        }
        else
        {
            if(redPoints>0)
            {
                pixFrame.botIndex=i;
            }
            ss+='w';
        }
    }
    qDebug()<<"pF:"<<ss;
    pixFrame.correctPixels = redPoints;
}

void ImageInterpreter::checkStep(int& destination)
{
    QPoint tempFrame[PIXFRAME_SIZE];

    for(int i=0;i<PIXFRAME_SIZE;i++)
    {
        tempFrame[i]=pixFrame.pixels[i];
        switch(destination)
        {
            case 1:
                moveFrameUp();
            break;
            case 2:
                moveFrameRight();
            break;
            case 3:
                moveFrameDown();
            break;
            case 4:
                moveFrameLeft();
            break;
        }
    }
    if(pixFrame.correctPixels<3)
    {
        for(int i=0;i<PIXFRAME_SIZE;i++)
        {
            pixFrame.pixels[i]=tempFrame[i];
        }
    }
}

/**
 * @brief Function correct frame
 */
void ImageInterpreter::correctFrame()
{
    if(pixFrame.topIndex==0 && pixFrame.botIndex<(PIXFRAME_SIZE-2))
    {
        if(pixFrame.orientation==Eorientation::horizontal)
        {
            moveFrameLeft();
        }
        else
        {
            moveFrameUp();
        }
    }
    else if(pixFrame.topIndex>0 && pixFrame.botIndex==(PIXFRAME_SIZE-1))
    {
        if(pixFrame.orientation==Eorientation::horizontal)
        {
            moveFrameRight();
        }
        else
        {
            moveFrameDown();
        }
    }
}

void ImageInterpreter::rotateFrame(QPoint rotatePoint)
{
    for(int i=0;i<PIXFRAME_SIZE;i++)
    {

    }
}

void ImageInterpreter::moveFrameUp()
{
    for(int i=0;i<PIXFRAME_SIZE;i++)
    {
        pixFrame.pixels[i]=QPoint(pixFrame.pixels[i].x(),pixFrame.pixels[i].y()-1);
    }
}

void ImageInterpreter::moveFrameDown()
{
    for(int i=0;i<PIXFRAME_SIZE;i++)
    {
        pixFrame.pixels[i]=QPoint(pixFrame.pixels[i].x(),pixFrame.pixels[i].y()+1);
    }
}

void ImageInterpreter::moveFrameRight()
{
    for(int i=0;i<PIXFRAME_SIZE;i++)
    {
        pixFrame.pixels[i]=QPoint(pixFrame.pixels[i].x()+1,pixFrame.pixels[i].y());
    }
}

void ImageInterpreter::moveFrameLeft()
{
    for(int i=0;i<PIXFRAME_SIZE;i++)
    {
        pixFrame.pixels[i]=QPoint(pixFrame.pixels[i].x()-1,pixFrame.pixels[i].y());
    }
}
