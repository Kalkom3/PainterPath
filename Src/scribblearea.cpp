#include "scribblearea.h"

#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>
#include <QKeyEvent>



ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    // Roots the widget to the top left even if resized
    setAttribute(Qt::WA_StaticContents);
    setFocusPolicy(Qt::ClickFocus);
    // Set defaults for the monitored variables
    modified = false;
    scribbling = false;
    startingPoint=false;
    myPenWidth = 5;
    myPenColor = Qt::magenta;
}

/**
 * @brief ScribbleArea::openImage function loading image from path
 * @param fileName
 * @return
 */
bool ScribbleArea::openImage(const QString &fileName)
{

    // If the image wasn't loaded leave this function
    if (!baseImage.load(fileName))
        return false;

    QSize newSize = baseImage.size();
    qDebug()<<newSize.width()<<" "<<newSize.height();
    resizeImage(&baseImage, newSize);
    image = baseImage;
    originalImageWidth=newSize.width();
    originalImageHeight=newSize.height();
    int tempWidth=newSize.width();
    int tempHeight=newSize.height();
    if(originalImageWidth>1500)
    {
        tempWidth=1500;
    }
    if(originalImageWidth>725)
    {
        tempHeight=725;
    }
    if( tempWidth==1500 || tempHeight==725)
    {
        image=baseImage.scaled(tempWidth,tempHeight,Qt::KeepAspectRatio);
        this->setFixedSize(tempWidth,tempHeight);
    }
    else
    {
        image = baseImage;
        this->setFixedSize( newSize);
    }

    startingPoint=false;
    modified = false;
    update();
    return true;
}


QPair<int, int> ScribbleArea::getOriginalDimensions()
{
    return QPair<int,int>(originalImageHeight,originalImageWidth);
}

QPoint ScribbleArea::getStartingPoint()
{
    return startingPointPos;
}

QImage& ScribbleArea::getImage()
{
    return image;
}

QImage &ScribbleArea::getBaseImage()
{
    return baseImage;
}

/**
 * @brief ScribbleArea::markCollision function changeing color of lines with colision to red
 * @param p1
 * @param p2
 */
void ScribbleArea::markCollision(QPoint p1, QPoint p2)
{
    myPenColor=Qt::red;
    drawLine(p1,p2);
    myPenColor=Qt::magenta;
}

/**
 * @brief ScribbleArea::clearImage function clearing drawing area
 */
void ScribbleArea::clearImage()
{
    if(!baseImage.isNull())
    {
       image=baseImage;
    }
    else
    {
        image.fill(qRgb(255, 255, 255));
    }
    pointsOnImage.clear();
    startingPoint=false;
    modified = true;

    update();
}

/**
 * @brief ScribbleArea::mousePressEvent event handler for mouse pressing
 * @param event
 */
void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        holdingPoint=-1;
        if(!startingPoint)
        {
            drawStartingPoint(event->pos());
            pointsOnImage.push_back(event->pos());
            emit pointAdded(event->pos());
        }
        else
        {
            if(image.pixelColor(event->pos())==Qt::blue || image.pixelColor(event->pos())==Qt::green)
            {
                holdingPoint = checkHoldingPoint(event->pos());
                if(holdingPoint!=-1)
                {
                    refreshImage();
                }
            }
            else
            {
                pointsOnImage.push_back(event->pos());
                refreshImage();
                emit pointAdded(event->pos());
            }
        }
    }
}

/**
 * @brief ScribbleArea::mouseMoveEvent event handler for mouse movement
 * @param event
 */
void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if(holdingPoint!=-1)
    {
        pointsOnImage[holdingPoint]=event->pos();
        refreshImage();
    }
}

/**
 * @brief ScribbleArea::mouseReleaseEvent event handler for mouse release
 * @param event
 */
void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(holdingPoint!=-1)
    {
        emit pointEdited(holdingPoint,event->pos());
    }
    holdingPoint=-1;

}

/**
 * @brief ScribbleArea::keyPressEvent event handler for keyboard pressing
 * @param event
 */
void ScribbleArea::keyPressEvent(QKeyEvent *event)
{

    if(event->key()==Qt::Key_Delete && holdingPoint!=-1)
    {
        emit pointDeleted(holdingPoint);
        pointsOnImage.erase(pointsOnImage.begin()+holdingPoint);
        refreshImage();
    }
}

/**
 * @brief ScribbleArea::paintEvent event handler for painting on image
 * @param event
 */
void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Returns the rectangle that needs to be updated
    QRect dirtyRect = event->rect();

    // Draws the rectangle where the image needs to
    // be updated
    painter.drawImage(dirtyRect, image, dirtyRect);
}

/**
 * @brief ScribbleArea::resizeEvent event handler for window resizeing
 * @param event
 */
void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

/**
 * @brief ScribbleArea::inRange function checking is first point is in specified range to second point
 * @param sourcePoint
 * @param pointToCheck
 * @param range
 * @return
 */
bool ScribbleArea::inRange(QPoint sourcePoint, QPoint pointToCheck, int range)
{
    if(abs(sourcePoint.x()-pointToCheck.x())<=range && abs(sourcePoint.y()-pointToCheck.y())<=range)
    {
        return true;
    }
    return false;
}

/**
 * @brief ScribbleArea::checkHoldingPoint function checking whitch point is being hold
 * @param mousePoint
 * @return
 */
int ScribbleArea::checkHoldingPoint(QPoint mousePoint)
{
    for(int i=0;i<pointsOnImage.size();i++)
    {
        if(inRange(pointsOnImage[i],mousePoint,myPenWidth+1))
        {
           return i;
        }
    }
    return -1;
}

/**
 * @brief ScribbleArea::drawLine function drawing line from point to point
 * @param startPoint
 * @param endPoint
 */
void ScribbleArea::drawLine(const QPoint &startPoint, const QPoint &endPoint)
{
    QPainter painter(&image);
    bool reDrawStart=false;
    if(image.pixelColor(startPoint)==Qt::green)
    {
        reDrawStart=true;
    }
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(startPoint, endPoint);

    painter.setPen(QPen(Qt::blue, myPenWidth+1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    painter.drawPoint(endPoint);
    painter.drawPoint(startPoint);

    if(reDrawStart)
    {
        QBrush brush(Qt::green);
        painter.fillRect(QRect(QPoint(startPoint.x()-12,startPoint.y()-12),QPoint(startPoint.x()+12,startPoint.y()+12)),brush);
    }
    else
    {
       painter.drawPoint(startPoint);
    }
    int rad = (myPenWidth / 2) + 2;

    update(QRect(startPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
}

/**
 * @brief ScribbleArea::drawStartingPoint function print starting point in first click position
 * @param endPoint
 */
void ScribbleArea::drawStartingPoint(const QPoint &endPoint)
{
    QPainter painter(&image);
    QBrush brush(Qt::green);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    startingPointPos=endPoint;
    painter.fillRect(QRect(QPoint(endPoint.x()-12,endPoint.y()-12),QPoint(endPoint.x()+12,endPoint.y()+12)),brush);
    modified = true;
    int rad = (myPenWidth / 2) + 2;

    update(QRect(endPoint.x()-15,endPoint.y()-15,30,30).normalized().adjusted(-rad, -rad, +rad, +rad));

    startingPoint=true;
}

/**
 * @brief ScribbleArea::resizeImage function resizeing image
 * @param image
 * @param newSize
 */
void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    // Check if we need to redraw the image
    if (image->size() == newSize)
        return;

    // Create a new image to display and fill it with white
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));

    // Draw the image
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

/**
 * @brief ScribbleArea::refreshImage function updating changes on image
 */
void ScribbleArea::refreshImage()
{
    image=baseImage;
    update();
    drawStartingPoint(pointsOnImage[0]);
    for(int i=0;i<pointsOnImage.size()-1;i++)
    {
        drawLine(pointsOnImage[i],pointsOnImage[i+1]);
    }
    lastPoint=pointsOnImage[pointsOnImage.size()-1];
}

