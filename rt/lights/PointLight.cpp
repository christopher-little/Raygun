#include "PointLight.h"


const static float eps = 0.0001f;


vector<Vector> PointLight::getSamplePoints(int sampleCount)
{
	// A point light source only has one point
	vector<Vector> points;
	points.push_back(_p);

	return points;
}