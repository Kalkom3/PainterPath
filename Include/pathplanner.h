#ifndef PATHPLANNER_H
#define PATHPLANNER_H

#include <QObject>
#include <QPoint>
#include "dimensiondata.h"

struct PathStep{
    float distance;
    float rotation;
};

class PathPlanner : public QObject
{
    Q_OBJECT
public:
    explicit PathPlanner(QObject *parent = nullptr);
    void parsePath();

    PathStep getStepData(int index);
    int getStepDataSize();
    DimensionData* getDimensions();
    QVector<QPoint> getPathPoints();
public slots:
    void AddStep(QPoint step);
    void EditStep(int step,QPoint newPos);
    void DeleteStep(int step);
    void clearPathPoints();
private:
    void AddPathStep(float rotation,float distance);
    float calculateLineFactor(QPoint point1,QPoint point2);
    void parseStep(int currentStep);
    QVector<QPoint>pathPoints;
    QVector<PathStep>pathSteps;
    QPair<float,float>Scales;
    DimensionData* dimensions;

};

#endif // PATHPLANNER_H
