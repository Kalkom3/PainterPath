#ifndef PATHPLANER_H
#define PATHPLANER_H

#include<QPair>
#include<QPoint>
#include<QVector>
#include<QImage>
#include<QObject>
class ImageInterpreter;
class DimensionData;

struct pathStep
{
    int destination;
    float distance;
};

class PathPlaner : public QObject
{
    Q_OBJECT
public:
    PathPlaner();
    bool processPath(QImage& img);
    void setDimensions(float realH, float realW, int imgH,int imgW);
    void setStartingPos(QPoint pos);

    QPoint getStartingPos();
public slots:
    void addStep(QPoint step);
private:
    QImage* map;
    QPoint startingPos;
    QVector<QPoint>pathSteps;
    QPair<float,float>Scales;
    DimensionData* dimensions;
    ImageInterpreter* imageParser;
};

#endif // PATHPLANER_H
