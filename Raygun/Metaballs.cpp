#include "Metaballs.h"

#include <vector>

using namespace std;

const float eps = 0.0001f;


bool Metaballs::intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v)
{
	vector<Vector> points;

	for(int i=0; i<balls.size(); i++)
	{
		Vector e_c = r.e() - balls[i].first;
		float A = r.d().dot(r.d());
		float B = r.d().dot(e_c);
		float C = e_c.dot(e_c) - balls[i].second*balls[i].second;

		// If discriminant is negative, no intersection!
		float discrim = B*B - A*C;
		if(discrim < eps)
			continue;

		// Determine nearest (lowest) intersection distance that occurs in front of the ray origin
		float sqrtDiscrim = sqrt(discrim);
		float near_t = (-B - sqrtDiscrim) / A;
		float far_t = (-B + sqrtDiscrim) / A;
	
		//p = r.e() + r.d()*t;
		//n = (p-balls[i].first).normalized();
	}

	return false;
}