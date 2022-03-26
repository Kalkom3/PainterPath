#ifndef DIMENSIONDATA_H
#define DIMENSIONDATA_H
#include<QPair>

class DimensionData
{
public:
    DimensionData();
    void setRealDim(float height,float width);
    void setImageDim(int height,int width);

    QPair<float,float> getScaleFactor();
    QPair<float,float> getRealDimensions();
private:
    void calculateScale();

    QPair<float,float> realDim;
    QPair<int,int> imageDim;
    QPair<float,float> scaleFactors;
};

#endif // DIMENSIONDATA_H
