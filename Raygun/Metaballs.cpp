#include "Metaballs.h"

#include <vector>

using namespace std;

const float eps = 0.0001f;


bool Metaballs::intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v)
{
	// Transform the ray to the local transformation
	Ray rt = _transform.inverse() * Ray(r);

	// Iterate over the metaballs and find an intersection
	int near_i = -1;
	float near_t=-1.0;
	float temp_t=-1.0;

	for(unsigned int i=0; i<balls.size(); i++)
	{
		Vector e_c = rt.e() - balls[i].first;
		float A = rt.d().dot(rt.d());
		float B = rt.d().dot(e_c);
		float C = e_c.dot(e_c) - balls[i].second*balls[i].second;

		// If discriminant is negative, no intersection!
		float discrim = B*B - A*C;
		if(discrim < eps)
			continue;

		// Determine nearest (lowest) intersection distance that occurs in front of the ray origin
		float sqrtDiscrim = sqrt(discrim);
		temp_t = (-B - sqrtDiscrim) / A;

		if(temp_t > 0.0 && (near_i < 0 || temp_t < near_t))
		{
			near_i = i;
			near_t = temp_t;
		}
	}

	if(near_i >= 0)
	{
		t = near_t;
		p = rt.e() + rt.d()*t;
		n = (p-balls[near_i].first).normalized();
		//*** u,v mapping not implemented here
		u = 0.0;
		v = 0.0;

		// Transform intersection point and normal to world space
		p = _transform.mult(p,1.0f);
		n = _transform.inverse().transpose().mult(n).normalized();

		return true;
	}
	else
		return false;
}
