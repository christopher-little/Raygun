#pragma once

#ifndef _PERLIN_
#define _PERLIN_

#include <vector>

#include "Vector.h"

using namespace std;

class Perlin
{
public:
	// Initialize the perlin noise generator with the given number of octaves/harmonics, and the power of 2 for the starting octave (with amplitude 1.0)
	Perlin(int octaves, int power);
	~Perlin();

	// Grab a noise value (assumes x,y,z are in range [0.0..1.0]
	float noise(float x, float y, float z);
private:
	int _w,_h,_d;	// Width height and depth of 3D grid of gradient vectors
	vector<vector<Vector>> _octaves; // List of harmonics. Each harmonic/octave contains a 3D grid of 3D gradient vectors
	int _basePower;	// Starting power of 2 for the dimension size of the gradient grid
};

#endif