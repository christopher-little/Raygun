#include "Mesh.h"

// epsilon - allowable margin of error ( 0.999999 == 1.0 )
static float eps = 0.00001f;

Mesh::Mesh()
{
	vertsPerFace = -1;
}

Mesh::~Mesh()
{

}

bool Mesh::intersect(const Ray &r, float &t, Vector &p, Vector &n)
{
	// Call intersection method for triangles or quads
	if(vertsPerFace == 3)
		return intersectTri(r, t, p, n);
	else if(vertsPerFace == 4)
		return intersectQuad(r, t, p, n);
	return false;
}


bool Mesh::intersectTri(const Ray &r, float &t, Vector &p, Vector &n)
{
	return false;
}


// This intersection method is more or less copied from "An Efficient Ray-Quadrilateral Intersection Test" by Lagae and Dutre
bool Mesh::intersectQuad(const Ray &r, float &t, Vector &p, Vector &n)
{
	// Check each face for intersection
	// -Recall faces[0-3] make up the 4 vertices of the first face, listed counter-clockwise
	int nearest_quad = -1;
	float nearest_t = -1.0;
	Vector nearest_n;
	float nearest_t_temp;
	Vector nearest_n_temp;
	for(int i=0; i<(int)faces.size(); i+=4)
	{
		Vector e_01 = vertices[faces[i+1]] - vertices[faces[i]];
		Vector e_03 = vertices[faces[i+3]] - vertices[faces[i]];
		Vector p = r.d().cross(e_03);
		float det = e_01.dot(p);
		if(std::abs(det) < eps)
			continue;//return false;
		float inv_det = 1.0f/det;
		Vector T = r.e() - vertices[faces[i]];
		float alpha = T.dot(p) * inv_det;
		if(alpha < 0.0f)
			continue;//return false;
		Vector Q = T.cross(e_01);
		float beta = r.d().dot(Q) * inv_det;
		if(beta < 0.0f)
			continue;//return false;

		if((alpha + beta) > 1.0f)
		{
			Vector e_23 = vertices[faces[i+3]] - vertices[faces[i+2]];
			Vector e_21 = vertices[faces[i+1]] - vertices[faces[i+2]];
			Vector p_prime = r.d().cross(e_21);
			float det_prime = e_23.dot(p_prime);
			if(std::abs(det_prime) < eps)
				continue;//return false;
			float inv_det_prime = 1.0f/det_prime;
			Vector T_prime = r.e() - vertices[faces[i+2]];
			float alpha_prime = T_prime.dot(p_prime) * inv_det_prime;
			if(alpha_prime < 0.0f)
				continue;//return false;
			Vector Q_prime = T_prime.cross(e_23);
			float beta_prime = r.d().dot(Q_prime) * inv_det_prime;
			if(beta_prime < 0.0f)
				continue;//return false;
		}

		nearest_t_temp = e_03.dot(Q) * inv_det;
		if(nearest_t_temp > 0.0 && (nearest_quad < 0 || nearest_t_temp < nearest_t))
		{
			nearest_quad = i; // Not important, just keeps track that a "nearest" quad has been intersected
			nearest_t = nearest_t_temp;
			nearest_n = e_01.cross(e_03).normalized();
		}
	}

	if(nearest_quad >= 0)
	{
		t = nearest_t;
		p = r.e() + r.d()*nearest_t;
		n = nearest_n;
		return true;
	}
	else
		return false;
}