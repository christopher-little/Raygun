#include "Sphere.h"

extern const float epsilon;
extern const float pi;


Sphere::Sphere(const Vector &c, float r)
{
    _c = c;
    _r = r;
}
Sphere::~Sphere()
{

}


bool Sphere::intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v)
{
    // Transform the ray to the local transformation
    Ray rt = _transform.inverse() * Ray(r);

    Vector e_c = rt.e() - _c;
    float A = rt.d().dot(rt.d());
    float B = rt.d().dot(e_c);
    float C = e_c.dot(e_c) - _r*_r;

    // If discriminant is negative, no intersection!
    float discrim = B*B - A*C;
    if(discrim < epsilon)
        return false;

    // Determine nearest (lowest) intersection distance that occurs in front of the ray origin
    float sqrtDiscrim = sqrt(discrim);
    float near_t = (-B - sqrtDiscrim) / A;
    if(near_t > epsilon)
        t = near_t;
    else
    {
        float far_t = (-B + sqrtDiscrim) / A;
        if(far_t > epsilon)
        {
            t = far_t;
        }
        else
            return false;
    }

    // Find the local intersection and normal with the sphere
    p = rt.e() + rt.d()*t;
    n = (p-_c).normalized();

    // u,v coordinates
    float ph = acos(-(p.y() - _c.y()) / _r);
    float th = atan2(-(p.z() - _c.z()), p.x() - _c.x());
    u = 0.5*(th/pi) + 0.5;
    v = ph/pi;

    // Transform intersection point and normal to world space
    p = _transform.mult(p,1.0f);
    n = _transform.inverse().transpose().mult(n).normalized();

    return true;
}
