#pragma once

#ifndef RAY
#define RAY

#include "Vector.h"

class Ray
{
public:
	Ray(Vector e, Vector d) : _e(e), _d(d) {}
	~Ray() {}

	inline Vector e() const { return _e; }
	inline Vector d() const { return _d; }

private:
	Vector _e;		// Ray origin position
	Vector _d;		// Ray direction
};

#endif