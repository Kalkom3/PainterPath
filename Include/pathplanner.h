#ifndef PATHPLANNER_H
#define PATHPLANNER_H

#include <QObject>
#include <QPoint>
class DimensionData;

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
public slots:
    void AddStep(QPoint step);
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
