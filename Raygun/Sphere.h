#pragma once

#ifndef SPHERE
#define SPHERE

#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere(Vector &c, float r);
	~Sphere();

	bool intersect(const Ray &r, float &t, Vector &p, Vector &n);

private:
	Vector _c;	// Centre position of sphere
	float  _r;	// Radius
};

#endif