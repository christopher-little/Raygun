#pragma once

#ifndef MATRIX
#define MATRIX

#include <cstring>
#include <cmath>

#include "Vector.h"
#include "Ray.h"

#include "TRACE.h"


// A class for a 4x4 matrix.
class Matrix
{
public:
	// Matrix is initialized to an identity matrix
	Matrix()
	{
		makeIdentity();
	}
	// Initialize matrix with given values
	Matrix( float m11, float m12, float m13, float m14,
					float m21, float m22, float m23, float m24,
					float m31, float m32, float m33, float m34,
					float m41, float m42, float m43, float m44)
	{
		_m[0]  = m11;
		_m[1]  = m12;
		_m[2]  = m13;
		_m[3]  = m14;
		_m[4]  = m21;
		_m[5]  = m22;
		_m[6]  = m23;
		_m[7]  = m24;
		_m[8]  = m31;
		_m[9]  = m32;
		_m[10] = m33;
		_m[11] = m34;
		_m[12] = m41;
		_m[13] = m42;
		_m[14] = m43;
		_m[15] = m44;
	}
	~Matrix() {}


	// Make an identity matrix
	void makeIdentity()
	{
		for(int x=0; x<4; x++) for(int y=0; y<4; y++)
		{
			if(x == y)
				_m[y*4 + x] = 1.0f;
			else
				_m[y*4 + x] = 0.0f;
		}
	}
	// Make a translation matrix
	// 1 0 0 x
	// 0 1 0 y
	// 0 0 1 z
	// 0 0 0 1
	void makeTranslate(float x, float y, float z)
	{
		makeIdentity();
		_m[3] = x;
		_m[7] = y;
		_m[11] = z;
	}
	// Make a rotation matrix for rotation of theta redians around an arbitrary axis
	void makeRotate(Vector &rotAxis, float theta)
	{
		makeIdentity();
		float c = cos(theta);
		float s = -sin(theta);
		float t = 1.0f-cos(theta);
		float x = rotAxis.x();
		float y = rotAxis.y();
		float z = rotAxis.z();
		
		_m[0] = t*x*x + c;
		_m[1] = t*x*y - s*z;
		_m[2] = t*x*z + s*y;
		
		_m[4] = t*x*y + s*z;
		_m[5] = t*y*y + c;
		_m[6] = t*y*z - s*x;

		_m[8] = t*x*z - s*y;
		_m[9] = t*y*z + s*x;
		_m[10] = t*z*z + c;
	}
	// Make a scaling matrix
	void makeScale(float x, float y, float z)
	{
		makeIdentity();
		_m[0] = x;
		_m[5] = y;
		_m[10] = z;
	}

	


	// Return the inverse matrix
	Matrix inverse()
	{
		float inv[16], det;
		int i;

		inv[0] =   _m[5]*_m[10]*_m[15] - _m[5]*_m[11]*_m[14] - _m[9]*_m[6]*_m[15]
		+ _m[9]*_m[7]*_m[14] + _m[13]*_m[6]*_m[11] - _m[13]*_m[7]*_m[10];
		inv[4] =  -_m[4]*_m[10]*_m[15] + _m[4]*_m[11]*_m[14] + _m[8]*_m[6]*_m[15]
		- _m[8]*_m[7]*_m[14] - _m[12]*_m[6]*_m[11] + _m[12]*_m[7]*_m[10];
		inv[8] =   _m[4]*_m[9]*_m[15] - _m[4]*_m[11]*_m[13] - _m[8]*_m[5]*_m[15]
		+ _m[8]*_m[7]*_m[13] + _m[12]*_m[5]*_m[11] - _m[12]*_m[7]*_m[9];
		inv[12] = -_m[4]*_m[9]*_m[14] + _m[4]*_m[10]*_m[13] + _m[8]*_m[5]*_m[14]
		- _m[8]*_m[6]*_m[13] - _m[12]*_m[5]*_m[10] + _m[12]*_m[6]*_m[9];
		inv[1] =  -_m[1]*_m[10]*_m[15] + _m[1]*_m[11]*_m[14] + _m[9]*_m[2]*_m[15]
		- _m[9]*_m[3]*_m[14] - _m[13]*_m[2]*_m[11] + _m[13]*_m[3]*_m[10];
		inv[5] =   _m[0]*_m[10]*_m[15] - _m[0]*_m[11]*_m[14] - _m[8]*_m[2]*_m[15]
		+ _m[8]*_m[3]*_m[14] + _m[12]*_m[2]*_m[11] - _m[12]*_m[3]*_m[10];
		inv[9] =  -_m[0]*_m[9]*_m[15] + _m[0]*_m[11]*_m[13] + _m[8]*_m[1]*_m[15]
		- _m[8]*_m[3]*_m[13] - _m[12]*_m[1]*_m[11] + _m[12]*_m[3]*_m[9];
		inv[13] =  _m[0]*_m[9]*_m[14] - _m[0]*_m[10]*_m[13] - _m[8]*_m[1]*_m[14]
		+ _m[8]*_m[2]*_m[13] + _m[12]*_m[1]*_m[10] - _m[12]*_m[2]*_m[9];
		inv[2] =   _m[1]*_m[6]*_m[15] - _m[1]*_m[7]*_m[14] - _m[5]*_m[2]*_m[15]
		+ _m[5]*_m[3]*_m[14] + _m[13]*_m[2]*_m[7] - _m[13]*_m[3]*_m[6];
		inv[6] =  -_m[0]*_m[6]*_m[15] + _m[0]*_m[7]*_m[14] + _m[4]*_m[2]*_m[15]
		- _m[4]*_m[3]*_m[14] - _m[12]*_m[2]*_m[7] + _m[12]*_m[3]*_m[6];
		inv[10] =  _m[0]*_m[5]*_m[15] - _m[0]*_m[7]*_m[13] - _m[4]*_m[1]*_m[15]
		+ _m[4]*_m[3]*_m[13] + _m[12]*_m[1]*_m[7] - _m[12]*_m[3]*_m[5];
		inv[14] = -_m[0]*_m[5]*_m[14] + _m[0]*_m[6]*_m[13] + _m[4]*_m[1]*_m[14]
		- _m[4]*_m[2]*_m[13] - _m[12]*_m[1]*_m[6] + _m[12]*_m[2]*_m[5];
		inv[3] =  -_m[1]*_m[6]*_m[11] + _m[1]*_m[7]*_m[10] + _m[5]*_m[2]*_m[11]
		- _m[5]*_m[3]*_m[10] - _m[9]*_m[2]*_m[7] + _m[9]*_m[3]*_m[6];
		inv[7] =   _m[0]*_m[6]*_m[11] - _m[0]*_m[7]*_m[10] - _m[4]*_m[2]*_m[11]
		+ _m[4]*_m[3]*_m[10] + _m[8]*_m[2]*_m[7] - _m[8]*_m[3]*_m[6];
		inv[11] = -_m[0]*_m[5]*_m[11] + _m[0]*_m[7]*_m[9] + _m[4]*_m[1]*_m[11]
		- _m[4]*_m[3]*_m[9] - _m[8]*_m[1]*_m[7] + _m[8]*_m[3]*_m[5];
		inv[15] =  _m[0]*_m[5]*_m[10] - _m[0]*_m[6]*_m[9] - _m[4]*_m[1]*_m[10]
		+ _m[4]*_m[2]*_m[9] + _m[8]*_m[1]*_m[6] - _m[8]*_m[2]*_m[5];

		det = _m[0]*inv[0] + _m[1]*inv[4] + _m[2]*inv[8] + _m[3]*inv[12];
		if (det == 0)
		{
			TRACE("Matrix: Warning: determinant = 0, returning identity matrix\n");
			return Matrix();
		}

		det = 1.0f / det;

		Matrix out;
		for (i = 0; i < 16; i++)
			out._m[i] = inv[i] * det;

		return out;
	}

	// Return the matrix transpose
	Matrix transpose()
	{
		Matrix out;
		for(int row=0; row<4; row++) for(int col=0; col<4; col++)
		{
			out._m[col*4 + row] = _m[row*4 + col];
		}

		return out;
	}




	// 4x4 Matrix multiply
	Matrix& operator *=(const Matrix &m)
	{
		float *out = new float[16];
		for(int row=0; row<4; row++) for(int col=0; col<4; col++)
		{
			int i=row*4;
			out[row*4 + col] =	_m[i]*m._m[col] +
													_m[i+1]*m._m[4 + col] +
													_m[i+2]*m._m[8 + col] +
													_m[i+3]*m._m[12 + col];
		}

		memcpy(_m, out, sizeof(float)*16);
		delete [] out;

		return *this;
	}

	
	// Multiply Matrix by ray (two column vectors; point and direction)
	Ray operator *(Ray &r)
	{
		float e[4],d[4];
		for(int row=0; row<4; row++)
		{
			int i=row*4;
			e[row]= _m[i]*r.e().x() +
							_m[i+1]*r.e().y() +
							_m[i+2]*r.e().z() +
							_m[i+3]*1.0f;
			d[row]= _m[i]*r.d().x() +
							_m[i+1]*r.d().y() +
							_m[i+2]*r.d().z() +
							_m[i+3]*0.0f;
		}

		return Ray(Vector(e[0],e[1],e[2]), Vector(d[0],d[1],d[2]));
	}


	// Multiply Matrix by column vector, w=0.0 for direction vector, w=1.0 for point (i.e. intersection point)
	//*** Need to replace w with w component of Vector class
	Vector mult(Vector &v, float w=0.0f)
	{
		float out[4];
		for(int row=0; row<4; row++)
		{
			int i=row*4;
			out[row]= _m[i]*v.x() +
								_m[i+1]*v.y() +
								_m[i+2]*v.z() +
								_m[i+3]*w;
		}

		return Vector(out[0], out[1], out[2]);
	}




	// Translate
	void translate(float x, float y, float z)
	{
		Matrix trans;
		trans.makeTranslate(x,y,z);
		(*this) *= trans;
	}

	// Rotate theta degrees around the axis rotAxis
	void rotate(Vector &rotAxis, float theta)
	{
		Matrix rot;
		// Make sure the rotation axis is normalized and theta is converted to radians
		rot.makeRotate(rotAxis.normalized(), theta*(3.14159f/180.0f));
		(*this) *= rot;
	}

	// Scale the x,y,z axes
	void scale(float x, float y, float z)
	{
		Matrix sc;
		sc.makeScale(x,y,z);
		(*this) *= sc;
	}



	// Dump the matrix as text
	void print()
	{
		TRACE("-------\n");
		for(int row=0; row<4; row++)
			TRACE("%f %f %f %f\n", _m[row*4], _m[row*4+1], _m[row*4+2], _m[row*4+3]);
		TRACE("-------\n");
	}



private:
	float _m[16];
};

#endif