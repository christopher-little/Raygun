#include "shapes/Cylinder.h"

#include <cmath>
#include <iostream>

using namespace std;

extern const float epsilon;


Cylinder::Cylinder(const Vector &c, float r, float h)
    : _c(c), _r(r), _h(h)
{

}

Cylinder::~Cylinder()
{}


// Intersection code is derived from source at http://www.geometrictools.com/LibMathematics/Intersection/Wm5IntrLine3Cylinder3.cpp
bool Cylinder::intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v)
{
    // Have to consider:
    // 1 - Ray is parallel to cylinder axis (y-axis), only check for cylinder cap intersections.
    // 2 - Ray is perpendicular to cylinder axis (parallel to x-z plane), only check for cylinder side/contour intersections.
    // 3 - Ray intersects both the side and an end cap.

    if(fabs(r.d().y()) > 1.0 - epsilon)
    {
        // Ray is parallel to cylinder axis
        return false;
    }

    Vector e_c = r.e() - _c;
    float A = r.d().x()*r.d().x() + r.d().z()*r.d().z();
    float B = e_c.x()*r.d().x() + e_c.z()*r.d().z();
    float C = e_c.x()*e_c.x() + e_c.z()*e_c.z() - _r*_r;

    float discrim = B*B - A*C;
    if(discrim < 0.0)
        return false;

    t = (-B - sqrt(discrim)) / A;
    p = r.e() + r.d()*t;
    if(p.y() < _c.y() || p.y() > _c.y()+_h)
        return false;
    Vector pSubC = p - _c;
    n = Vector(pSubC.x(), 0.0, pSubC.z()).normalized();

    return true;


    /*
    float rSqr = _r*_r;
    float halfHeight = 0.5*_h;
    Vector e_c = r.e() - _c;


    // CONSIDERATION 1
    // If ray direction is parallel to cylinder axis (y-axis), only check for end cap intersections.
    if(fabs(r.d().y()) >= 1.0-epsilon)
    {
        float radialSqrDist = rSqr - e_c.x()*e_c.x() - e_c.z()*e_c.z();
        if(radialSqrDist < 0.0)
        {
            return false;
        }

        if(r.d().y() > 0.0)
        {
            // The ray intersects the "bottom" cap followed by the "top" cap
            t = -e_c.y() - halfHeight;
            p = r.e() + r.d()*t;
            n = Vector(0.0,-1.0,0.0);
            //***t_far = -e_c.y() + halfHeight;
        }
        else
        {
            // The ray intersects the "top" cap followed by the "bottom" cap
            t = e_c.y() - halfHeight;
            p = r.e() + r.d()*t;
            n = Vector(0.0,1.0,0.0);
            //***t_far = e_c.y() + halfHeight;
        }

        return true;
    }



    float A, B, C, discrim;

    // CONSIDERATION 2
    // If the ray direction is perpendicular to the cylinder axis, only check for intersection with cylinder sides
    if(fabs(r.d().y()) <= epsilon)
    {
        // Ensure ray origin is between cylinder end cap planes
        if(fabs(e_c.y()) > halfHeight)
        {
            return false;
        }

        // Substitute ray equation into cylinder equation and solve for t
        A = r.d().x()*r.d().x() + r.d().z()*r.d().z();
        B = e_c.x()*r.d().x() + e_c.z()*r.d().z();
        C = e_c.x()*e_c.x() + e_c.z()*e_c.z() - rSqr;

        discrim = B*B - A*C;

        if(discrim < 0.0)
            return false;
        else if(discrim > epsilon)
        {
            t = (-B - sqrt(discrim)) / A;
            //t_far = (-B + sqrt(discrim)) / A;
            p = r.e() + r.d()*t;
            n = Vector(p.x()-_c.x(), 0.0, p.z()-_c.z()).normalized();

            return true;
        }
        else
        {
            // Ray is tangent to (grazes) the cylinder, only 1 intersection
            t = -B/A;
            p = r.e() + r.d()*t;
            n = Vector(p.x()-_c.x(), 0.0, p.z()-_c.z()).normalized();

            return true;
        }
    }



    int quantity = 0;
    float tList[2]; // List of intersections found

    // CONSIDERATION 3
    // Ray may intersect zero, one or both end caps.
    float tTop, tBot; // Distance where ray intersects top and bottom cap PLANE (not necessarily within the cylinder radius).

    tTop = (-halfHeight - e_c.y()) / r.d().y();
    float xTmp = e_c.x() + r.d().x()*tTop;
    float zTmp = e_c.z() + r.d().z()*tTop;
    if(xTmp*xTmp + zTmp*zTmp <= rSqr)
    {
        // Intersection with top cap
        tList[quantity++] = tTop;
    }

    tBot = (halfHeight - e_c.y()) / r.d().y();
    xTmp = e_c.x() + r.d().x()*tBot;
    zTmp = e_c.z() + r.d().z()*tBot;
    if(xTmp*xTmp + zTmp*zTmp <= rSqr)
    {
        // Intersection with top cap
        tList[quantity++] = tBot;
    }

    if(quantity == 2)
    {
        // Ray intersected both end caps
        n = Vector(0.0,1.0,0.0);
        if(tList[0] > tList[1])
        {
            float save = tList[0];
            tList[0] = tList[1];
            tList[1] = save;

            n = Vector(0.0,-1.0,0.0);
        }

        t = tList[0];
        p = r.e() + r.d()*t;
        return true;
    }



    // Consideration 4
    // Ray intersects only cylinder sides, or a side and a cap.
    A = r.d().x()*r.d().x() + r.d().z()*r.d().z();
    B = e_c.x()*r.d().x() + e_c.z()*r.d().z();
    C = e_c.x()*e_c.x() + e_c.z()*e_c.z() - rSqr;

    discrim = B*B - A*C;

    if(discrim < 0.0)
    {
        if(quantity != 0)
        {
            //***Unexpected condition! (Ray intersected 1 end cap but NOT the cylinder wall)
        }
        return false;
    }
    else if(discrim > epsilon)
    {
        float root = sqrt(discrim);
        float inv = 1.0/A;
        float tValue = (-B - root) * inv;
        if(tTop <= tBot)
        {
            if(tTop <= tValue && tValue <= tBot)
            {
                tList[quantity++] = tValue;
            }
        }
        else
        {
            if(tBot <= tValue && tValue <= tTop)
            {
                tList[quantity++] = tValue;
            }
        }

        if(quantity == 2)
        {
            // Ray intersects an end cap and the wall
            if(tList[0] > tList[1])
            {
                // Ray first intersects the wall followed by a cap
                float save = tList[0];
                tList[0] = tList[1];
                tList[1] = save;

                t = tList[0];
                p = r.e() + r.d()*t;
                n = Vector(p.x()-_c.x(), 0.0, p.z()-_c.z()).normalized();
            }
            else
            {
                // Ray first intersects a cap followed by the wall
                t = tList[0];
                p = r.e() + r.d()*t;
                n = Vector(0.0,1.0,0.0);
                //***No simple way to determine if bottom/top cap was intersected, if ray points "upward", flip normal vector to point "downward"
                if(r.d().y() > 0.0)
                    n = n*-1.0;
            }

            return true;
        }


        tValue = (-B + root) * inv;
        if(tTop <= tBot)
        {
            if(tTop <= tValue && tValue <= tBot)
            {
                tList[quantity++] = tValue;
            }
        }
        else
        {
            if(tBot <= tValue && tValue <= tTop)
            {
                tList[quantity++] = tValue;
            }
        }
    }
    else
    {
        float tValue = -B/A;
        if(tTop <= tBot)
        {
            if(tTop <= tValue && tValue <= tBot)
            {
                tList[quantity++] = tValue;
            }
        }
        else
        {
            if(tBot <= tValue && tValue <= tTop)
            {
                tList[quantity++] = tValue;
            }
        }
    }


    // At this point, the only intersections that have occured are 1 or 2 intersections with the cylinder wall
    if(quantity == 2)
    {
        if(tList[0] > tList[1])
        {
            float save = tList[0];
            tList[0] = tList[1];
            tList[1] = save;
        }
    }


    t = tList[0];
    p = r.e() + r.d()*t;
    n = Vector(p.x()-_c.x(), 0.0, p.z()-_c.z()).normalized();

    return true;
    */
}
