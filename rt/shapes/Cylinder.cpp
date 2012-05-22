#include "shapes/Cylinder.h"


Cylinder::Cylinder(const Vector &c, float r, float h)
    : _c(c), _r(r), _h(h){

}

Cylinder::~Cylinder() {}


bool Cylinder::intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v){
    return false;
}
