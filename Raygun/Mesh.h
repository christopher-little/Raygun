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
	inline void addVertex(const Vector &v) { vertices.push_back(v); }
	// Append a face vertex index
	inline void addFacePoint(int i) { faces.push_back(i); }
	// Append a vertex uv coordinate
	inline void addUVCoord(float u, float v) { uvList.push_back(u); uvList.push_back(v); }

	
	bool intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v);

private:
	int vertsPerFace; // 3 for triangle mesh, 4 for quad mesh
	std::vector<Vector> vertices; // List of vertices
	std::vector<int> faces; // List of faces (number of components depends on tri vs quad
	std::vector<float> uvList; // List of uv coordinates for each vertex

	//***This needs list of normals and u,v coordinates
	
	// Specialized intersection methods for triangles and quads
	bool intersectTri(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v);
	bool intersectQuad(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v);
};

#endif
