#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H


#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class ScribbleArea : public QWidget
{

    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = 0);

    // Handles all events
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);

    // Has the image been modified since last save
    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }
    QPair<int,int>getOriginalDimensions();
    QPoint getStartingPoint();
    QImage& getImage();
public slots:

    // Events to handle
    void clearImage();
    void print();
signals:
    void pointAdded(QPoint point);
protected:
    //Mouse events
    void mousePressEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void drawStartingPoint(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    int originalImageHeight;
    int originalImageWidth;
    bool startingPoint;
    bool scribeLock;
    bool modified;
    bool scribbling;
    int myPenWidth;
    QColor myPenColor;
    QImage image;
    QImage baseImage;
    QPoint lastPoint;
    QPoint startingPointPos;
};
#endif // SCRIBBLEAREA_H
