#include "imageinterpreter.h"

#include<pathplanner.h>
#include<QDebug>
#include<QVector>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/imgproc.hpp>

ImageInterpreter::ImageInterpreter(PathPlanner& pathPlaner, QObject *parent):
    QObject{parent}, path(pathPlaner)
{

}


void ImageInterpreter::setImg(QString path)
{


}

void ImageInterpreter::setBaseImg(QImage &bImage)
{
    baseImage=&bImage;
}

bool ImageInterpreter::checkPath()
{
    bool collision=false;
    QVector<QPoint>stepPoints=path.getPathPoints();
    QVector<QPoint>collisionPoints;
    for(int i=0;i<stepPoints.size()-1;i++)
    {
        if(checkCollisionInLine(stepPoints[i],stepPoints[i+1]))
        {
            collisionPoints.push_back(stepPoints[i]);
            collision=true;
        }

    }
    return collision;
}



bool ImageInterpreter::checkCollisionInLine(QPoint startPoint,QPoint endPoint)
{
    double a = endPoint.y() - startPoint.y();
    double b = startPoint.x() - endPoint.x();
    double c = a * (startPoint.x()) + b * (startPoint.y());

    QPoint lowerPoint = startPoint.y()>endPoint.y() ? endPoint : startPoint;
    QPoint higherPoint = startPoint.y()>endPoint.y() ? startPoint : endPoint;
    QPoint currentPoint;
    for(int i=lowerPoint.y();i<higherPoint.y();i++)
    {
        currentPoint = QPoint(((c-(b*i))/a),i);

        if(checkCollision(currentPoint))
        {
            emit collisionDetected(startPoint,endPoint);
            return true;
        }
    }
    return false;
}


bool ImageInterpreter::checkCollision(QPoint centerPoint)
{
    for(int i=-10;i<10;i++)
    {
        if(centerPoint.x()+i<0 || centerPoint.y()+i<0)
        {
            continue;
        }
        if(baseImage->pixelColor(QPoint(centerPoint.x()+i,centerPoint.y()))==Qt::black)
        {
            return true;
        }

        if(baseImage->pixelColor(QPoint(centerPoint.x(),centerPoint.y()+i))==Qt::black)
        {
            return true;
        }
    }
    return false;
}



