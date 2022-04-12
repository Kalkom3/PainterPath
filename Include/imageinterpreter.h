#ifndef IMAGEINTERPRETER_H
#define IMAGEINTERPRETER_H

#include<QObject>
#include<QImage>


class PathPlanner;



class ImageInterpreter : public QObject
{
    Q_OBJECT

public:

    explicit ImageInterpreter(PathPlanner& pathPlaner,QObject *parent = nullptr);
    void setImg(QString path);
    void setBaseImg(QImage& bImage);
    bool checkPath();

signals:

    void collisionDetected(QPoint p1 ,QPoint p2);

private:

    PathPlanner& path;
    QImage* image;
    QImage* baseImage;
    bool checkCollisionInLine(QPoint startPoint,QPoint endPoint);

    bool checkCollision(QPoint centerPoint);
};

#endif // IMAGEINTERPRETER_H
