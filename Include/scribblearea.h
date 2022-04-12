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


    QPair<int,int>getOriginalDimensions();
    QPoint getStartingPoint();
    QImage& getImage();
    QImage& getBaseImage();

public slots:
    void markCollision(QPoint,QPoint);
    void clearImage();

signals:
    void pointAdded(QPoint point);
    void pointEdited(int pointNr,QPoint newPos);
    void pointDeleted(int pointNr);

protected:
    //Mouse events
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    //Keyboar events
    void keyPressEvent(QKeyEvent *event) override;

    //Other events
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    bool inRange(QPoint sourcePoint, QPoint pointToCheck, int range);
    int checkHoldingPoint(QPoint mousePoint);
    void drawLine(const QPoint &startPoint, const QPoint &endPoint);
    void drawStartingPoint(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    void refreshImage();

    QVector<QPoint>pointsOnImage;
    int originalImageHeight;
    int originalImageWidth;
    bool startingPoint;
    bool scribeLock;
    bool modified;
    bool scribbling;
    int holdingPoint;
    bool refresh;
    int myPenWidth;
    QColor myPenColor;
    QImage image;
    QImage baseImage;
    QPoint lastPoint;
    QPoint startingPointPos;
};
#endif // SCRIBBLEAREA_H
