#include "pathplanner.h"
#include <QDebug>
PathPlanner::PathPlanner(QObject *parent)
    : QObject{parent}
{
    dimensions = new DimensionData;
}

void PathPlanner::AddStep(QPoint step)
{
    pathPoints.push_back(step);
}

void PathPlanner::AddPathStep(float rotation, float distance)
{
    pathSteps.push_back({distance,rotation});
}

float PathPlanner::calculateLineFactor(QPoint point1, QPoint point2)
{
    float dx=point2.x()-point1.x();
    float dy=point2.y()-point1.y();
    return (dy/dx);
}

void PathPlanner::parseStep(int currentStep)
{

    if(currentStep>=pathPoints.size() || currentStep==0)
    {
        qDebug()<<"Step Error";
        return;
    }
    QPoint lastPoint=pathPoints[currentStep-1];
    QPoint currentPoint=pathPoints[currentStep];
    float dx=lastPoint.x()-currentPoint.x();
    float dy=lastPoint.y()-currentPoint.y();
    float distance;
    float rotation =  atan(calculateLineFactor(lastPoint,currentPoint))*180/3.1415;
    if(dy>0)
    {
        if(dx>0)
        {
            rotation+=180;
        }
        else
        {
            rotation=360+rotation;
        }
    }
    else
    {
        if(dx>0)
        {
            rotation=180+rotation;
        }
    }

    if(dx!=0 && dy!=0)
    {
        distance=sqrt(abs(pow(dy*Scales.second,2)+pow(dx*Scales.first,2)));
    }
    else
    if(dx==0)
    {
        distance=abs(dy)*Scales.second;
    }
    else
    {
        distance=abs(dx)*Scales.first;
    }
    qDebug()<<"Rot="<<rotation;
    qDebug()<<"dis="<<distance;
    AddPathStep(rotation,distance);


}

void PathPlanner::parsePath()
{
    Scales=dimensions->getScaleFactor();
    for(int i = 1;i<pathPoints.size();i++)
    {
        parseStep(i);
    }
}

PathStep PathPlanner::getStepData(int index)
{
    return pathSteps[index];
}

int PathPlanner::getStepDataSize()
{
    return pathSteps.size();
}

DimensionData* PathPlanner::getDimensions()
{
    return dimensions;
}
