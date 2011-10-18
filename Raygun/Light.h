#pragma once

#ifndef LIGHT
#define LIGHT

#include <vector>

using namespace std;

class Light
{
public:
	Light(){}
	~Light(void) {}


	// Get the light colour value
	Colour c() { return _c; }

	// Return a bunch of points on the light surface for the ray tracer to sample
	virtual vector<Vector> getSamplePoints(int sampleCount) = 0;

protected:
	Colour _c;	// Light emittance colour
};

#endif