#include "dimensiondata.h"

DimensionData::DimensionData()
{
    realDim=QPair<float,float>(0,0);
}

void DimensionData::setRealDim(float height,float width)
{
    realDim=QPair<float,float>(height,width);
}

void DimensionData::setImageDim(int height,int width)
{
    imageDim=QPair<int,int>(height,width);
}

void DimensionData::calculateScale()
{
    float xScale = realDim.first/imageDim.first;
    float yScale = realDim.second/imageDim.second;

    scaleFactors=QPair<float,float>(xScale,yScale);
}

QPair<float, float> DimensionData::getScaleFactor()
{
    calculateScale();
    return scaleFactors;
}

QPair<float, float> DimensionData::getRealDimensions()
{
    return realDim;
}
