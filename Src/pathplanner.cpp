#include "pathplanner.h"
#include <QDebug>

PathPlanner::PathPlanner(QObject *parent)
    : QObject{parent}
{
    dimensions = new DimensionData;
}

/**
 * @brief PathPlanner::AddStep function adding point to points vector
 * @param step
 */
void PathPlanner::AddStep(QPoint step)
{
    pathPoints.push_back(step);
}

/**
 * @brief PathPlanner::EditStep function modifing point in points vector
 * @param step
 * @param newPos
 */
void PathPlanner::EditStep(int step, QPoint newPos)
{
    pathPoints[step]=newPos;
}

/**
 * @brief PathPlanner::DeleteStep function removing point form points vector
 * @param step
 */
void PathPlanner::DeleteStep(int step)
{
    pathPoints.erase(pathPoints.begin()+step);
}

/**
 * @brief PathPlanner::AddPathStep function adding new step to path
 * @param rotation
 * @param distance
 */
void PathPlanner::AddPathStep(float rotation, float distance)
{
    pathSteps.push_back({distance,rotation});
}

/**
 * @brief PathPlanner::calculateLineFactor function calculate line factor for line equasion
 * @param point1
 * @param point2
 * @return
 */
float PathPlanner::calculateLineFactor(QPoint point1, QPoint point2)
{
    float dx=point2.x()-point1.x();
    float dy=point2.y()-point1.y();
    return (dy/dx);
}

/**
 * @brief PathPlanner::parseStep function converting 2 points from vector to step for path
 * @param currentStep
 */
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
    AddPathStep(rotation,distance);
}

/**
 * @brief PathPlanner::parsePath function convert points vector to path of distances and rotations
 */
void PathPlanner::parsePath()
{
    pathSteps.clear();
    Scales=dimensions->getScaleFactor();
    for(int i = 1;i<pathPoints.size();i++)
    {
        parseStep(i);
    }
}

/**
 * @brief PathPlanner::clearPathPoints function clearing points vector
 */
void PathPlanner::clearPathPoints()
{
    pathPoints.clear();
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

QVector<QPoint> PathPlanner::getPathPoints()
{
    return pathPoints;
}
