#pragma once

#ifndef METABALLS
#define METABALLS

#include "shapes/Shape.h"

#include <vector>
#include "Vector.h"

using namespace std;

// The Metaballs class handles iso-surfaces made up of a set of spheres
class Metaballs : public Shape
{
public:
    Metaballs(float threshold) { _threshold = threshold; }
    ~Metaballs()
    {
        balls.clear();
    }

    // Add a sphere (given as point and radius) to the set of metaballs
    inline void addSphere(const Vector &c, float r)
    {
        balls.push_back(pair<Vector,float>(c,r));
    }



    bool intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v);

private:
    vector< pair<Vector,float> > balls;	// Set of spheres defining the "blobs" of the metaballs shape

    float _threshold;	// Energy threshold of iso-surface


    // Return the normal vector (gradient) at point p
    Vector normal(const Vector &p);
};

#endif
