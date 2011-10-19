#pragma once

#ifndef AABB
#define AABB

#include "Vector.h"

////////////////////////////////////////////////////////////////////////////////
// Axis aligned bounding box utility functions
// 
// - Provides various intersection functions for aabb's
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Methods to handle box-triangle intersection, based on code written by Moller
////////////////////////////////////////////////////////////////////////////////

/*
#define FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;
*/
inline void FINDMINMAX(float x0, float x1, float x2, float &min, float &max)
{
	min = max = x0;
	if(x1<min) min=x1;
	if(x1>max) max=x1;
	if(x2<min) min=x2;
	if(x2>max) max=x2;
}


//======================== X-tests ========================
/*
#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*dA.y - b*dA.z;			       	   \
	p2 = a*dC.y - b*dC.z;			       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxHalfLengths.y + fb * boxHalfLengths.z;   \
	if(min>rad || max<-rad) return false;
*/
inline bool AXISTEST_X01(float a, float b, float fa, float fb, const Vector &dA, const Vector &dC, const Vector &boxHalfLengths, float min, float max, float &p0, float &p2, float &rad)
{
	p0 = a*dA.y() - b*dA.z();
	p2 = a*dC.y() - b*dC.z();
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;}
	rad = fa * boxHalfLengths.y() + fb * boxHalfLengths.z();
	if(min>rad || max<-rad) return false;
	else return true;
}

/*
#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*dA.y - b*dA.z;			           \
	p1 = a*dB.y - b*dB.z;			       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxHalfLengths.y + fb * boxHalfLengths.z;   \
	if(min>rad || max<-rad) return false;
*/
inline bool AXISTEST_X2(float a, float b, float fa, float fb, const Vector &dA, const Vector &dB, const Vector &boxHalfLengths, float min, float max, float &p0, float &p1, float rad)
{
	p0 = a*dA.y() - b*dA.z();
	p1 = a*dB.y() - b*dB.z();
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;}
	rad = fa * boxHalfLengths.y() + fb * boxHalfLengths.z();
	if(min>rad || max<-rad) return false;
	else return true;
}

//======================== Y-tests ========================
/*
#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*dA.x + b*dA.z;		      	   \
	p2 = -a*dC.x + b*dC.z;	       	       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxHalfLengths.x + fb * boxHalfLengths.z;   \
	if(min>rad || max<-rad) return false;
*/
inline bool AXISTEST_Y02(float a, float b, float fa, float fb, const Vector &dA, const Vector &dC, const Vector &boxHalfLengths, float min, float max, float &p0, float &p2, float &rad)
{
	p0 = -a*dA.x() + b*dA.z();
	p2 = -a*dC.x() + b*dC.z();
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;}
	rad = fa * boxHalfLengths.x() + fb * boxHalfLengths.z();
	if(min>rad || max<-rad) return false;
	else return true;
}

/*
#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*dA.x + b*dA.z;		      	   \
	p1 = -a*dB.x + b*dB.z;	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxHalfLengths.x + fb * boxHalfLengths.z;   \
	if(min>rad || max<-rad) return false;
*/
inline bool AXISTEST_Y1(float a, float b, float fa, float fb, const Vector &dA, const Vector &dB, const Vector &boxHalfLengths, float min, float max, float &p0, float &p1, float rad)
{
	p0 = -a*dA.x() + b*dA.z();		      	   \
	p1 = -a*dB.x() + b*dB.z();	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxHalfLengths.x() + fb * boxHalfLengths.z();   \
	if(min>rad || max<-rad) return false;
	else return true;
}

//======================== Z-tests ========================
/*
#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*dB.x - b*dB.y;			           \
	p2 = a*dC.x - b*dC.y;			       	   \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
	rad = fa * boxHalfLengths.x + fb * boxHalfLengths.y;   \
	if(min>rad || max<-rad) return false;
*/
inline bool AXISTEST_Z12(float a, float b, float fa, float fb, const Vector &dB, const Vector &dC, const Vector &boxHalfLengths, float min, float max, float &p1, float &p2, float &rad)
{
	p1 = a*dB.x() - b*dB.y();
	p2 = a*dC.x() - b*dC.y();
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;}
	rad = fa * boxHalfLengths.x() + fb * boxHalfLengths.y();
	if(min>rad || max<-rad) return false;
	else return true;
}

/*
#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*dA.x - b*dA.y;				   \
	p1 = a*dB.x - b*dB.y;			           \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxHalfLengths.x + fb * boxHalfLengths.y;   \
	if(min>rad || max<-rad) return false;
*/
inline bool AXISTEST_Z0(float a, float b, float fa, float fb, const Vector &dA, const Vector &dB, const Vector &boxHalfLengths, float min, float max, float &p0, float &p1, float rad)
{
	p0 = a*dA.x() - b*dA.y();
	p1 = a*dB.x() - b*dB.y();
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;}
	rad = fa * boxHalfLengths.x() + fb * boxHalfLengths.y();
	if(min>rad || max<-rad) return false;
	else return true;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Given a point and normal vector making a plane and the half side lengths of an axis aligned bounding
// box, return true if the plane overlaps the bounding box
// NOTE not a general purpose function, only useful when used from triBoxOverlap function
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool boxOverlapPlane(const Vector &normal, float d, const Vector &boxHalfLengths)
{
	Vector vmin, vmax;
	//X-Axis
	if(normal.x() > 0.0f)
	{
		vmin.x(-boxHalfLengths.x());
		vmax.x(boxHalfLengths.x());
	}
	else
	{
		vmin.x(boxHalfLengths.x());
		vmax.x(-boxHalfLengths.x());
	}
	//Y-Axis
	if(normal.y() > 0.0f)
	{
		vmin.y(-boxHalfLengths.y());
		vmax.y(boxHalfLengths.y());
	}
	else
	{
		vmin.y(boxHalfLengths.y());
		vmax.y(-boxHalfLengths.y());
	}
	//Z-Axis
	if(normal.z() > 0.0f)
	{
		vmin.z(-boxHalfLengths.z());
		vmax.z(boxHalfLengths.z());
	}
	else
	{
		vmin.z(boxHalfLengths.z());
		vmax.z(-boxHalfLengths.z());
	}

	if(normal.dot(vmin)+d > 0.0f) return false;
	if(normal.dot(vmax)+d >= 0.0f) return true;

	return false;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Given the centre position of a axis aligned bounding box, the half length of each side and the 
// vertices of a triangle (A, B, C), return true if the triangle overlaps the box
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool boxOverlapTriangle(const Vector &boxCenter, const Vector &boxHalfLengths, const Vector &A, const Vector &B, const Vector &C)
{
	Vector dA, dB, dC; //Triangle vertices after moving box to origin (v)
	dA = A - boxCenter;
	dB = B - boxCenter;
	dC = C - boxCenter;

	Vector edgeA, edgeB, edgeC; //Triangle edges (e)
	edgeA = dB - dA;
	edgeB = dC - dB;
	edgeC = dA - dC;

	// Perform 9 tests
	Vector temp;
	float p0, p1, p2, rad, min, max;
	temp = Vector(fabs(edgeA.x()), fabs(edgeA.y()), fabs(edgeA.z()));
	if(!AXISTEST_X01(edgeA.z(),edgeA.y(), temp.z(),temp.y(), dA, dC, boxHalfLengths, min, max, p0, p2, rad))
		return false;
	if(!AXISTEST_Y02(edgeA.z(), edgeA.x(), temp.z(), temp.x(), dA, dC, boxHalfLengths, min, max, p0, p2, rad))
		return false;
	if(!AXISTEST_Z12(edgeA.y(), edgeA.x(), temp.y(), temp.x(), dB, dC, boxHalfLengths, min, max, p1, p2, rad))
		return false;

	temp = Vector(fabs(edgeB.x()), fabs(edgeB.y()), fabs(edgeB.z()));
	if(!AXISTEST_X01(edgeB.z(), edgeB.y(), temp.z(), temp.y(), dA, dC, boxHalfLengths, min, max, p0, p2, rad))
		return false;
	if(!AXISTEST_Y02(edgeB.z(), edgeB.x(), temp.z(), temp.x(), dA, dC, boxHalfLengths, min, max, p0, p2, rad))
		return false;
	if(!AXISTEST_Z0(edgeB.y(), edgeB.x(), temp.y(), temp.x(), dA, dB, boxHalfLengths, min, max, p0, p1, rad))
		return false;

	temp = Vector(fabs(edgeC.x()), fabs(edgeC.y()), fabs(edgeC.z()));
	if(!AXISTEST_X2(edgeC.z(), edgeC.y(), temp.z(), temp.y(), dA, dB, boxHalfLengths, min, max, p0, p1, rad))
		return false;
	if(!AXISTEST_Y1(edgeC.z(), edgeC.x(), temp.z(), temp.x(), dA, dB, boxHalfLengths, min, max, p0, p1, rad))
		return false;
	if(!AXISTEST_Z12(edgeC.y(), edgeC.x(), temp.y(), temp.x(), dB, dC, boxHalfLengths, min, max, p1, p2, rad))
		return false;


	// Perform 3 tests
	FINDMINMAX(dA.x(), dB.x(), dC.x(), min, max);
	if(min > boxHalfLengths.x() || max < -boxHalfLengths.x()) return 0;

	FINDMINMAX(dA.y(), dB.y(), dC.y(), min, max);
	if(min > boxHalfLengths.y() || max < -boxHalfLengths.y()) return 0;

	FINDMINMAX(dA.z(), dB.z(), dC.z(), min, max);
	if(min > boxHalfLengths.z() || max < -boxHalfLengths.z()) return 0;


	// Perform 1 test
	Vector normal;
	float d;
	normal = edgeA.cross(edgeB);
	d=-normal.dot(dA);
	//Check if box intersects plane of triangle
	if(!boxOverlapPlane(normal,d,boxHalfLengths)) return false;

	// Box and triangle overlaps
	return true;
}

	
	
	
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Find where ray enters and exits the cube specified by min and max.
// enter/exit_face -> index of face where ray enters and exits.
//						-X:0, +X:1, -Y:2, +Y:3, -Z:4, +Z:5
// enter/exit_t -> distance along ray where enter and exits cube
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool rayBoxIntersect(const Vector &e, const Vector &d,
							const Vector &min, const Vector &max,
							int &enter_face, int &exit_face,
							float &enter_t, float &exit_t)
{
	// Boolean values true if interval is +inf,-inf or -inf,+inf
	bool infX = false;
	bool infY = false;
	bool infZ = false;
	// Initialize max and min interval values for each axis
	float tXMin = 0;
	float tXMax = 0;
	float tYMin = 0;
	float tYMax = 0;
	float tZMin = 0;
	float tZMax = 0;
	// Check if any of the intervals are guaranteed not to overlap the volume ie [-inf,-inf] or [+inf,+inf]
	if(d.x() == 0 && (min.x() > 0 && max.x() > 0 || min.x() < 0 && max.x() < 0) ||
		d.y() == 0 && (min.y() > 0 && max.y() > 0 || min.y() < 0 && max.y() < 0) ||
		d.z() == 0 && (min.z() > 0 && max.z() > 0 || min.z() < 0 && max.z() < 0))
	{
		return false;
	}
	// Otherwise check for divide by 0, resulting in valid infinite intervals
	else
	{
		if(d.x() == 0)
		{
			infX = true;
		}
		if(d.y() == 0)
		{
			infY = true;
		}
		if(d.z() == 0)
		{
			infZ = true;
		}
	}

	// Keep track of nearest/furthest intersection and face where it occurs
	int faceNear = -1, faceFar = -1;
	float tNear = 0, tFar = 0;
	int faceNearTemp, faceFarTemp; // assume low face is near face unless ray direction requires otherwise
	bool tInf=true;
	// Calculate absolute intevals when not dividing by 0
	if(!infX)
	{
		faceNearTemp = 0;
		faceFarTemp = 1;
		tXMin = (min.x() - e.x()) / d.x();
		tXMax = (max.x() - e.x()) / d.x();
		if(d.x() < 0)
		{
			faceNearTemp = 1;
			faceFarTemp = 0;
			float temp = tXMin;
			tXMin = tXMax;
			tXMax = temp;
		}

		faceNear = faceNearTemp;
		faceFar = faceFarTemp;
		tNear = tXMin;
		tFar = tXMax;
		tInf = false;
	}

	if(!infY)
	{
		faceNearTemp = 2;
		faceFarTemp = 3;
		tYMin = (min.y() - e.y()) / d.y();
		tYMax = (max.y() - e.y()) / d.y();
		if(d.y() < 0)
		{
			faceNearTemp = 3;
			faceFarTemp = 2;
			float temp = tYMin;
			tYMin = tYMax;
			tYMax = temp;
		}
		
		if(tInf)
		{
			faceNear = faceNearTemp;
			faceFar = faceFarTemp;
			tNear = tYMin;
			tFar = tYMax;
			tInf = false;
		}
		else
		{
			if(tYMin > tNear)
			{
				faceNear = faceNearTemp;
				tNear = tYMin;
			}
			if(tYMax < tFar)
			{
				faceFar = faceFarTemp;
				tFar = tYMax;
			}
		}
	}
	if(!infZ)
	{
		faceNearTemp = 4;
		faceFarTemp = 5;
		tZMin = (min.z() - e.z()) / d.z();
		tZMax = (max.z() - e.z()) / d.z();
		if(d.z() < 0)
		{
			faceNearTemp = 5;
			faceFarTemp = 4;
			float temp = tZMin;
			tZMin = tZMax;
			tZMax = temp;
		}
		
		if(tInf)
		{
			faceNear = faceNearTemp;
			faceFar = faceFarTemp;
			tNear = tZMin;
			tFar = tZMax;
			tInf = false;
		}
		else
		{
			if(tZMin > tNear)
			{
				faceNear = faceNearTemp;
				tNear = tZMin;
			}
			if(tZMax < tFar)
			{
				faceFar = faceFarTemp;
				tFar = tZMax;
			}
		}
	}

	// If the near intersection is closer than the far intersection or the ray
	// enters the cube behind an object intersection
	if(tNear > tFar)
	{
		return false;
	}
	else
	{
		enter_face = faceNear;
		exit_face = faceFar;
		enter_t = tNear;
		exit_t = tFar;
		return true;
	}
}

#endif