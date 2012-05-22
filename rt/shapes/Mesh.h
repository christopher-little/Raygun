#pragma once

#ifndef MESH
#define MESH

#include <vector>

#include "shapes/Shape.h"
#include "Vector.h"
#include "Ray.h"


////////////////////////////////////////////////////////////////////////////////
// class Mesh
//
// Container for triangle mesh data (vertices, faces, normals and vertex uv coordinates)
////////////////////////////////////////////////////////////////////////////////
class Mesh
{
public:
    Mesh(){}
    ~Mesh()
    {
        vertices.clear();
        faces.clear();
        uvList.clear();
        //normals.clear();
    }


    // Create a square mesh
    static Mesh *square();
    // Create a diamond mesh
    static Mesh *diamond();


    // Append a vertex to the vertices list
    inline void addVertex(const Vector &v) { vertices.push_back(v); }
    // Append a face vertex index
    inline void addFacePoint(int i) { faces.push_back(i); }
    // Append a vertex uv coordinate
    inline void addUVCoord(float u, float v) { uvList.push_back(u); uvList.push_back(v); }


    std::vector<Vector> vertices;
    std::vector<int> faces;
    std::vector<float> uvList;
    //std::vector<Vector> normals;
};



////////////////////////////////////////////////////////////////////////////////
// class MeshShape
//
// Shape container ray tracing the contained triangle mesh
////////////////////////////////////////////////////////////////////////////////
class MeshShape : public Shape
{
public:
    MeshShape(Mesh *m);
    ~MeshShape();

    bool intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v);

private:
    int vertsPerFace; // 3 for triangle mesh, 4 for quad mesh
//	std::vector<Vector> vertices; // List of vertices
//	std::vector<int> faces; // List of faces (number of components depends on tri vs quad
//	std::vector<float> uvList; // List of uv coordinates for each vertex

    Mesh *mesh;

    //***This needs list of normals

    // Specialized intersection methods for triangles and quads
    bool intersectTri(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v);
    bool intersectQuad(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v);
};

#endif
