#pragma once

#ifndef MESH
#define MESH

#include <vector>

#include "Shape.h"
#include "Vector.h"
#include "Ray.h"

////////////////////////////////////////////////////////////////////////////////
// class Mesh
//
// Container for triangle or quad list mesh
////////////////////////////////////////////////////////////////////////////////
class Mesh : public Shape
{
public:
	Mesh();
	~Mesh();
	

	// Set the number of vertices per face
	inline void setVertsPerFace(int c) { vertsPerFace = c; }
	
	// Append a vertex to the vertices list
	inline void addVertex(Vector &v) { vertices.push_back(v); }
	// Append a face vertex index
	inline void addFacePoint(int i) { faces.push_back(i); }

	
	bool intersect(const Ray &r, float &t, Vector &p, Vector &n);

private:
	int vertsPerFace; // 3 for triangle mesh, 4 for quad mesh, etc.
	std::vector<Vector> vertices; // List of vertices
	std::vector<int> faces; // List of faces (number of components depends on tri vs quad
	
	// Specialized intersection methods for triangles and quads
	bool intersectTri(const Ray &r, float &t, Vector &p, Vector &n);
	bool intersectQuad(const Ray &r, float &t, Vector &p, Vector &n);
};

#endif