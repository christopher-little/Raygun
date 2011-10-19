#pragma once

#ifndef VECTOR
#define VECTOR

#include <cmath>
#include <cstdio>

////////////////////////////////////////////////////////////////////////////////
// class Vector
//
// Container for a 3D vector
////////////////////////////////////////////////////////////////////////////////
class Vector
{
public:
	// Constructors
	Vector() : _x(0.0), _y(0.0), _z(0.0), _w(1.0) {}
	Vector(float inx, float iny, float inz) : _x(inx), _y(iny), _z(inz), _w(1.0) {}

	// Return respective component
	inline float x() const { return _x; }
	inline float y() const { return _y; }
	inline float z() const { return _z; }
	inline float w() const { return _w; }

	// Set respective component
	inline void x(float c) { _x = c; }
	inline void y(float c) { _y = c; }
	inline void z(float c) { _z = c; }


	// Sum two vectors
	inline Vector operator+ (Vector v) const { return Vector(_x+v._x, _y+v._y, _z+v._z); }
	// Subtract two vectors
	inline Vector operator- (Vector v) const { return Vector(_x-v._x, _y-v._y, _z-v._z); }
	// Scale by a scalar value
	inline Vector operator* (float s) const { return Vector(_x*s, _y*s, _z*s); }


	// Calculate length/magnitude of vector
	inline float length() const { return sqrt(_x*_x + _y*_y + _z*_z); }
	// Return this vector normalized
	inline Vector normalized() const
	{
		float len = length();
		return Vector( _x/len, _y/len, _z/len );
	}


	// Vector operations
	inline float dot(Vector v) const { return _x*v._x + _y*v._y + _z*v._z; }
	inline Vector cross(Vector v) const { return Vector(_y*v._z - _z*v._y,  _z*v._x - _x*v._z,  _x*v._y - _y*v._x); }


	// Convert Vector to char* for printing, etc.
	operator char *() const
	{
		char *vecString = new char[128];
		sprintf(vecString,"(%f, %f, %f)", _x, _y, _z);
		return vecString;
	}

private:
	float _x,_y,_z,_w; // 3D vector components, plus w for homogeneous coordinate
};

#endif