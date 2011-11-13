#include "Mesh.h"

// epsilon - allowable margin of error ( 0.999999 == 1.0 )
static float eps = 0.0001f;

Mesh::Mesh()
{
	vertsPerFace = -1;
}

Mesh::~Mesh()
{

}



bool Mesh::intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v)
{
	// Call intersection method for triangles or quads
	if(vertsPerFace == 3)
		return intersectTri(r, t, p, n, u, v);
	else if(vertsPerFace == 4)
		return intersectQuad(r, t, p, n, u, v);
	return false;
}




bool Mesh::intersectTri(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v)
{
	// Transform the ray for the local transformation matrix
	Ray rt = _transform.inverse() * Ray(r);

	Vector A, B, C;
	float nearest_t = -1.0f, temp_t;
	Vector nearest_n;
	int face_index = -1; // Index of intersected face
	int temp_index;
	for(int faceI=0; faceI<faces.size(); faceI+=3)
	{
		A = vertices[faces[faceI]];
		B = vertices[faces[faceI+1]];
		C = vertices[faces[faceI+2]];

		// Skip face if triangle plane and ray are parallel
		float temp = ((B-A).cross(C-A)).dot(rt.d());
		if(temp < eps && temp > -eps)
			continue;
		else
		{
			double a = A.x() - B.x();
			double b = A.y() - B.y();
			double c = A.z() - B.z();
			double d = A.x() - C.x();
			double e = A.y() - C.y();
			double f = A.z() - C.z();
			double g = rt.d().x();
			double h = rt.d().y();
			double i = rt.d().z();
			double j = A.x() - rt.e().x();
			double k = A.y() - rt.e().y();
			double l = A.z() - rt.e().z();

			double eiSubhf = e*i - h*f;
			double gfSubdi = g*f - d*i;
			double dhSubeg = d*h - e*g;

			double M = a*eiSubhf + b*gfSubdi + c*dhSubeg;

			double akSubjb = a*k - j*b;
			double jcSubal = j*c - a*l;
			double blSubkc = b*l - k*c;

			// Gamma and Beta represent distance along vectors (c-a) and (b-a) respectively,
			// if Gamma or Beta are beyond their respective vectors, then intersection is not within the surface of the triangle
			double Gamma = (i*akSubjb + h*jcSubal + g*blSubkc)/M;
			if(Gamma < 0 || Gamma > 1)
				continue;

			double Beta = (j*eiSubhf + k*gfSubdi + l*dhSubeg)/M;
			if(Beta < 0 || Beta > (1-Gamma))
				continue;


			temp_t = -((f*akSubjb + e*jcSubal + d*blSubkc)/M);


			if(temp_t > eps && (nearest_t <= eps || temp_t < nearest_t))
			{
				nearest_t = temp_t;

				// Retain intersected face index (remember face is specified by i*3,i*3+1,i*3+2)
				face_index = faceI;
			}
		}
	}

	// Return intersection details
	if(nearest_t > eps)
	{
		t = nearest_t;
		p = r.e() + r.d()*t;

		// Get the intersected triangle vertices
		A = vertices[faces[face_index]];
		B = vertices[faces[face_index+1]];
		C = vertices[faces[face_index+2]];

		// Compute flat face normal
		n = _transform.inverse().transpose().mult((B-A).cross(C-A).normalized()).normalized();

		Vector pLocal = rt.e() + rt.d()*t;
		Vector v0 = C - A;
		Vector v1 = B - A;
		Vector v2 = pLocal - A;
		float dot00 = v0.dot(v0);
		float dot01 = v0.dot(v1);
		float dot02 = v0.dot(v2);
		float dot11 = v1.dot(v1);
		float dot12 = v1.dot(v2);
		float invDenom = 1.0f / (dot00*dot11 - dot01*dot01);
		float local_u = (dot11*dot02 - dot01*dot12) * invDenom;
		float local_v = (dot00*dot12 - dot01*dot02) * invDenom;
		
		// Recall faces[face_index] is the index for the vertex, faces[face_index]*2 then maps to the correct u,v position in the uvList since uvList=(u0,v0,u1,v1,...)
		int uv_index = faces[face_index]*2;
		float u_0 = uvList[uv_index];
		float v_0 = uvList[uv_index+1];
		uv_index = faces[face_index+1]*2;
		float u_1 = uvList[uv_index];
		float v_1 = uvList[uv_index+1];
		uv_index = faces[face_index+2]*2;
		float u_2 = uvList[uv_index];
		float v_2 = uvList[uv_index+1];

		float tu1 = u_1 - u_0;
		float tv1 = v_1 - v_0;
		float tu0 = u_2 - u_0;
		float tv0 = v_2 - v_0;

		u = u_0 + tu0*local_u + tu1*local_v;
		v = v_0 + tv0*local_u + tv1*local_v;

		return true;
	}
	else
		return false;
}




// This intersection method is more or less copied from "An Efficient Ray-Quadrilateral Intersection Test" by Lagae and Dutre
bool Mesh::intersectQuad(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v)
{
	// Check each face for intersection
	// -Recall faces[0-3] make up the 4 vertices of the first face, listed counter-clockwise
	int nearest_quad = -1;
	float nearest_t = -1.0;
	Vector nearest_n;
	float nearest_t_temp;
	Vector nearest_n_temp;
	// Store some extra information needed to calculate u,v coordinates
	float nearest_alpha, nearest_beta;
	Vector nearest_e_01, nearest_e_03;

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

		// Record the intersection point if it's the closest in the mesh
		nearest_t_temp = e_03.dot(Q) * inv_det;
		if(nearest_t_temp > 0.0 && (nearest_quad < 0 || nearest_t_temp < nearest_t))
		{
			nearest_quad = i; // Not important, just keeps track that a "nearest" quad has been intersected
			nearest_t = nearest_t_temp;
			nearest_n = e_01.cross(e_03).normalized();
			nearest_alpha = alpha;
			nearest_beta = beta;
			nearest_e_01 = e_01;
			nearest_e_03 = e_03;
		}
	}

	// Return the nearest quad intersection in the mesh
	if(nearest_quad >= 0)
	{
		t = nearest_t;
		p = r.e() + r.d()*nearest_t;
		n = nearest_n;

		// Compute u,v coordinates
		float alpha_11, beta_11;
		Vector E_01 = nearest_e_01;
		Vector E_03 = nearest_e_03;
		Vector E_02 = vertices[faces[nearest_quad+2]] - vertices[faces[nearest_quad]];

		if( std::abs(n.x()) >= std::abs(n.y()) && std::abs(n.x()) >= std::abs(n.z()))
		{
			alpha_11 = ((E_02.y() * E_03.z()) - (E_02.z() * E_03.y())) / n.x();
			beta_11  = ((E_01.y() * E_02.z()) - (E_01.z() * E_02.y())) / n.x();
		}
		else if( std::abs(n.y()) >= std::abs(n.x()) && std::abs(n.y()) >= std::abs(n.z()))
		{
			alpha_11 = ((E_02.z() * E_03.x()) - (E_02.x() * E_03.z())) / n.y();
			beta_11  = ((E_01.z() * E_02.x()) - (E_01.x() * E_02.z())) / n.y();
		}
		else
		{
			alpha_11 = ((E_02.x() * E_03.y()) - (E_02.y() * E_03.x())) / n.z();
			beta_11  = ((E_01.x() * E_02.y()) - (E_01.y() * E_02.x())) / n.z();
		}

		if(std::abs(alpha_11 - 1.0f) < eps)
		{
			u = nearest_alpha;
			if(std::abs(beta_11 - 1.0f) < eps)
				v = nearest_beta;
			else
				v = nearest_beta / ((u*(beta_11 - 1.0f)) + 1.0f);
		}
		else if (std::abs(beta_11 - 1.0f) < eps)
		{
			v = nearest_beta;
			u = nearest_alpha / ((v * (alpha_11 - 1.0f)) + 1.0f);
		}
		else
		{
			float A = 1.0f - beta_11;
			float B = (nearest_alpha * (beta_11 - 1.0f)) - (nearest_beta * (alpha_11 - 1.0f)) - 1.0f;
			float C = nearest_alpha;
			float D = (B * B) - (4.0f * A * C);
			float Q = -0.5f * (B + ((B < 0.0f ? -1.0f : 1.0f) * std::sqrt(D)));
			u = Q / A;
			if ((u < 0.0f) || (u > 1.0f))
				u = C / Q;
			v = nearest_beta / ((u * (beta_11 - 1.0f)) + 1.0f); 
		}

		return true;
	}
	else
		return false;
}