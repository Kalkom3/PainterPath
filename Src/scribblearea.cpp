#include "scribblearea.h"

#include <QtWidgets>
#include <QtPrintSupport/qtprintsupportglobal.h>
#include <QPrinter>
#include <QPrintDialog>




ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    // Roots the widget to the top left even if resized
    setAttribute(Qt::WA_StaticContents);

    // Set defaults for the monitored variables
    modified = false;
    scribbling = false;
    startingPoint=false;
    myPenWidth = 5;
    myPenColor = Qt::red;
}

// Used to load the image and place it in the widget
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

// Save the current image
bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
{
    // Created to hold the image
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

// Used to change the pen color
void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

// Used to change the pen width
void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
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

// Color the image area with white
void ScribbleArea::clearImage()
{
    if(baseImage.isNull())
    {
       image=baseImage;
    }
    else
    {
        image.fill(qRgb(255, 255, 255));
    }

    startingPoint=false;
    modified = true;
    update();
}

// If a mouse button is pressed check if it was the
// left button and if so store the current position
// Set that we are currently drawing
void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if(!startingPoint)
        {
            drawStartingPoint(event->pos());
        }
        else
        {
            drawLineTo(event->pos());
            lastPoint = event->pos();
            scribbling = true;
        }
        emit pointAdded(event->pos());
    }
}


// QPainter provides functions to draw on the widget
// The QPaintEvent is sent to widgets that need to
// update themselves
void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Returns the rectangle that needs to be updated
    QRect dirtyRect = event->rect();

    // Draws the rectangle where the image needs to
    // be updated
    painter.drawImage(dirtyRect, image, dirtyRect);
}

// Resize the image to slightly larger then the main window
// to cut down on the need to resize the image
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

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    bool reDrawStart=false;
    if(image.pixelColor(lastPoint)==Qt::green)
    {
        reDrawStart=true;
    }
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    painter.setPen(QPen(Qt::blue, myPenWidth+1, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawPoint(endPoint);

    if(reDrawStart)
    {
        QBrush brush(Qt::green);
        painter.fillRect(QRect(QPoint(lastPoint.x()-12,lastPoint.y()-12),QPoint(lastPoint.x()+12,lastPoint.y()+12)),brush);
    }
    else
    {
       painter.drawPoint(lastPoint);
    }
    modified = true;
    int rad = (myPenWidth / 2) + 2;

    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endPoint;
}

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

    update(QRect(endPoint.x()-15,endPoint.y()-15,30,30).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endPoint;
    startingPoint=true;
}

// When the app is resized create a new image using
// the changes made to the image
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

// Print the image
void ScribbleArea::print()
{
    // Check for print dialog availability
#if QT_CONFIG(printdialog)

    // Can be used to print
    QPrinter printer(QPrinter::HighResolution);

    // Open printer dialog and print if asked
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_CONFIG(printdialog)
}
