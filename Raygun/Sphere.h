#pragma once

#ifndef SPHERE
#define SPHERE

#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere(const Vector &c, float r);
	~Sphere();

	bool intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v);

private:
	Vector _c;	// Centre position of sphere
	float  _r;	// Radius
};

#endif
