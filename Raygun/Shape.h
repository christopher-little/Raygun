#pragma once

#ifndef SHAPE
#define SHAPE

#include "Ray.h"
#include "Material.h"

class Shape
{
public:
	Shape() { _mat = new Material(); }
	~Shape() {}

	// Get and set the material object
	inline Material* mat() const { return _mat; }
	inline void setMat(Material *mat)
	{
		_mat = mat;
	}

	// Intersects a ray with the shape object.
	// -Returns true if an intersection occurs
	// -Stores distance (t), intersection point (p), normal (n) and u,v coordinates in provided variables
	virtual bool intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v) = 0;
private:
	Material *_mat;
};

#endif