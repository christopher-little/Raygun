#pragma once

#ifndef POINT_LIGHT
#define POINT_LIGHT

#include "Vector.h"
#include "Colour.h"
#include "Light.h"

#include <vector>

using namespace std;

class PointLight : public Light
{
public:
	PointLight(const Vector &p, const Colour &c)
	{
		_p = p;
		_c = c;
	}

	// Just in case, return the light position
	Vector p() { return _p; }

	// Point light source has only one point to sample, so returned vector contains only one point.
	virtual vector<Vector> getSamplePoints(int sampleCount);

private:
	Vector _p;	// Point light position
};

#endif
