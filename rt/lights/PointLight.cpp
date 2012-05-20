#include "PointLight.h"


vector<Vector> PointLight::getSamplePoints(int sampleCount)
{
    // A point light source only has one point
    vector<Vector> points;
    points.push_back(_p);

    return points;
}
