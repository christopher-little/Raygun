#pragma once

#ifndef CAMERA
#define CAMERA

#include "Vector.h"

class Camera
{
public:
	Camera( Vector &position, Vector &direction, Vector &up, float &focalDist, float &w, float &h )
	{
		_e = position;
		_g = direction;
		_t = up;
		_N = focalDist;
		_w = w;
		_h = h;
	}
	~Camera() {}

	// Get camera properties
	inline Vector e() const { return _e; }
	inline Vector g() const { return _g; }
	inline Vector t() const { return _t; }
	inline float N() const { return _N; }
	inline float w() const { return _w; }
	inline float h() const { return _h; }

private:
	Vector    _e;	// Camera position (eye)
	Vector    _g;	// (G)aze direction
	Vector    _t;	// "Up" direction
	float     _N;	// Focal length (distance to image plane)
	float  _w,_h;	// Width and Height of image plane in metres (basically uSpan/vSpan in older ray tracers)
};

#endif