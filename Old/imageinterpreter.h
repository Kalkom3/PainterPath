#ifndef IMAGEINTERPRETER_H
#define IMAGEINTERPRETER_H

#include<QImage>

int constexpr PIXFRAME_SIZE = 7;

class PathPlaner;
enum Eorientation
{
    horizontal,
    vertical
};

struct PixFrame
{
    QPoint pixels[PIXFRAME_SIZE];
    int topIndex;
    int botIndex;
    int correctPixels;
    Eorientation orientation; //0-vertical,1-horizontal
};

struct PathStep
{

};

class ImageInterpreter
{
public:
    ImageInterpreter(PathPlaner& pathPlaner);
    void setImage(QImage& img);
    void processImage();
private:
    PixFrame pixFrame;
    bool checkBorders(QPoint point);
    int findDestination(QPoint startPoint,QPoint nextPoint);
    QPoint findFirstStep();
    void checkFrameStatus();
    void checkStep(int& destination);
    void correctFrame();
    void rotateFrame(QPoint rotatePoint);
    void moveFrameUp();
    void moveFrameDown();
    void moveFrameRight();
    void moveFrameLeft();
    QImage* image;
    PathPlaner& path;
};

#endif // IMAGEINTERPRETER_H
