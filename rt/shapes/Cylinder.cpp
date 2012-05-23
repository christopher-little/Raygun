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
    Vector e_c = r.e() - _c;
    float A = r.d().x()*r.d().x() + r.d().z()*r.d().z();
    float B = e_c.x()*r.d().x() + e_c.z()*r.d().z();
    float C = e_c.x()*e_c.x() + e_c.z()*e_c.z() - _r*_r;

    float discrim = B*B - A*C;
    if(discrim < 0.0)
        return false;

    t = (-B - sqrt(discrim)) / A;
    p = r.e() + r.d()*t;
    Vector pSubC = p - _c;
    n = Vector(pSubC.x(), 0.0, pSubC.z()).normalized();

    return true;

    /*
    Vector U(1.0,0.0,0.0);
    Vector V(0.0,1.0,0.0);
    Vector W(0.0,0.0,1.0);
    float halfHeight = 0.5*_h;
    float rSqr = _r*_r;

    Vector diff = r.e() - _c;
    Vector P(U.dot(diff), V.dot(diff), W.dot(diff));

    // Get y-value of incoming ray direction in cylinder coordinates ***This probably isn't necessary since U,V,W are the canonical axes
    float dy = V.dot(r.d());

    if(fabs(dy) >= 1.0 - epsilon)
    {
        // Ray direction is parallel to cylinder axis, check for intersection with cylinder caps
        float radialSqrDist = rSqr - P.x()*P.x() - P.z()*P.z();
        if(radialSqrDist < 0.0)
        {
            return false;
        }

        if(dy > 0.0)
        {
            t = -P.y() - halfHeight;
            n=Vector(0.0,-1.0,0.0);
            //***t_far = -P.y() + halfHeight;
        }
        else
        {
            t = P.y() - halfHeight;
            n=Vector(0.0,1.0,0.0);
            //***t_far = P.y() + halfHeight;
        }

        p = r.e() + r.d()*t;
        //***Still need to figure out u,v coords
        return true;
    }

    // Convert ray direction to cylinder coordinates ***This probably isn't necessary since U,V,W are the canonical axes
    Vector D(U.dot(r.d()), dy, W.dot(r.d()));

    float a0, a1, a2, discr, root, inv, tValue;

    if(fabs(D.y()) <= epsilon)
    {
        // The ray direction is perpendicular to the cylinder axis
        if(fabs(P.y()) > halfHeight)
        {
            // Ray does not travel between cylinder end cap planes
            return false;
        }

        // Test intersection of line P+t*D with infinite cylinder
        // x^2+z^2 = r^2.  This reduces to computing the roots of a
        // quadratic equation.  If P = (px,py,pz) and D = (dx,dy,dz),
        // then the quadratic equation is
        //   (dx^2+dz^2)*t^2 + 2*(px*dx+pz*dz)*t + (px^2+pz^2-r^2) = 0
        a0 = P.x()*P.x() + P.z()*P.z() - rSqr;
        a1 = P.x()*D.x() + P.z()*D.z();
        a2 = D.x()*D.x() + D.z()*D.z();
        discr = a1*a1 - a0*a2;
        if(discr < 0.0)
        {
            return false;
        }
        else if(discr > epsilon)
        {
            root = sqrt(discr);
            inv = 1.0/a2;
            t = (-a1 - root)*inv;
            //t_far = (-a1 + root)*inv;
            p = r.e()+r.d()*t;
            n = Vector(p.x()-_c.x(), p.y()-_c.y(), 0.0);
            return true;
        }
        else
        {
            // Ray is tangent (grazes) the cylinder
            t = -a1/a2;
            p = r.e()+r.d()*t;
            n = Vector(p.x()-_c.x(), 0.0, p.z()-_c.z());
            return true;
        }
    }


    int quantity = 0; // Number of intersections found
    float tList[2]; // List of intersections found thus far
    inv = 1.0/D.y();

    float t0 = (-halfHeight - P.y())*inv;
    float xTmp = P.x() + t0*D.x();
    float zTmp = P.z() + t0*D.z();
    if(xTmp*xTmp + zTmp*zTmp <= rSqr)
    {
        // Intersection with top cylinder cap
        tList[quantity++] = t0;
    }

    float t1 = (halfHeight - P.y())*inv;
    xTmp = P.x() + t1*D.x();
    zTmp = P.z() + t1*D.z();
    if(xTmp*xTmp + zTmp*zTmp <= rSqr)
    {
        // Intersection with bottom cylinder cap
        tList[quantity++] = t1;
    }

    if(quantity == 2)
    {
        // Ray intersected top and bottom caps (through the middle of the cylinder)
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


    // If quantity == 1 the ray intersects the cylinder side somewhere between the end caps
    a0 = P.x()*P.x() + P.z()*P.z() - rSqr;
    a1 = P.x()*D.x() + P.z()*D.z();
    a2 = D.x()*D.x() + D.z()*D.z();
    discr = a1*a1 - a0*a2;
    if(discr < 0.0)
    {
        // Ray does not intersect cylinder wall
        if(quantity != 0)
        {
            //***Unexpected condition! (Ray intersected 1 end cap but NOT the cylinder wall)
        }
        return false;
    }
    else if(discr > epsilon)
    {
        root = sqrt(discr);
        inv = 1.0/a2;
        tValue = (-a1 - root)*inv;
        if(t0 <= t1)
        {
            if(t0 <= tValue && tValue <= t1)
            {
                tList[quantity++] = tValue;
            }
        }
        else
        {
            if(t1 <= tValue && tValue <= t0)
            {
                tList[quantity++] = tValue;
            }
        }

        if(quantity == 2)
        {
            // Ray intersects an end cap and the cylinder wall
            if(tList[0] > tList[1])
            {
                // Cylinder wall intersection occurs BEFORE cap intersection
                float save = tList[0];
                tList[0] = tList[1];
                tList[1] = save;

                t = tList[0];
                p = r.e() + r.d()*t;
                n = Vector(p.x()-_c.x(), 0.0, p.z()-_c.z());
            }
            else
            {
                t = tList[0];
                p = r.e() + r.d()*t;
                n = Vector(0.0,1.0,0.0); //***First intersection in the tList is the end cap intersection, BUT it maybe the top OR bottom, for now assuming top thus +y normal vector
            }

            return true;
        }

        tValue = (-a1 + root)*inv;
        if(t0 <= t1)
        {
            if(t0 <= tValue && tValue <= t1)
            {
                tList[quantity++] = tValue;
            }
        }
        else
        {
            if(t1 <= tValue && tValue <= t0)
            {
                tList[quantity++] = tValue;
            }
        }
    }
    else
    {
        tValue = -a1/a2;
        if(t0 <= t1)
        {
            if(t0 <= tValue && tValue <= t1)
            {
                tList[quantity++] = tValue;
            }
        }
        else
        {
            if(t1 <= tValue && tValue <= t0)
            {
                tList[quantity++] = tValue;
            }
        }
    }


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
    n = Vector(p.x()-_c.x(), 0.0, p.z()-_c.z());

    return true;
    */
}
