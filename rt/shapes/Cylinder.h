#pragma once

#ifndef CYLINDER_H
#define CYLINDER_H

#include "shapes/Shape.h"


class Cylinder : public Shape
{
public:
    Cylinder(const Vector &c, float r, float h);
    ~Cylinder();

    bool intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v);

private:
    Vector  _c;	// Bottom centre position of cylinder
    float   _r;	// Radius
    float   _h; // Height/length of cylinder
};


#endif // CYLINDER_H
