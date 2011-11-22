#include "Metaballs.h"

#include <vector>
#include <algorithm>

using namespace std;

#define real double
const float eps = 0.0001f;


bool Metaballs::intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v)
{
	// Transform the ray to the local transformation.
	Ray rt = _transform.inverse() * r;

	vector<pair<float,float> > intersectionList;

	vector<pair<Vector,float> >::iterator it;
	Vector center;
	float radius;
	for(it=balls.begin(); it!=balls.end(); it++)
	{
		center = it->first;
		radius = it->second;

		Vector e_c = rt.e() - center;
		float A = rt.d().dot(rt.d());
		float B = rt.d().dot(e_c);
		float C = e_c.dot(e_c) - radius*radius;

		float discrim = B*B - A*C;
		if(discrim < eps)
			continue;

		float sqrtDiscrim, near_t, far_t;
		sqrtDiscrim = sqrt(discrim);
		near_t = (-B - sqrtDiscrim) / A;
		if(near_t > eps)
		{
			float previousEnergy = 0.0;
			for(float subRadius=0.95; subRadius > 0.1; subRadius-=0.01)
			{
				C = e_c.dot(e_c) - subRadius*subRadius;

				discrim = B*B - A*C;
				if(discrim < eps)
					break;

				sqrtDiscrim = sqrt(discrim);
				near_t = (-B - sqrtDiscrim) / A;
				far_t = (-B + sqrtDiscrim) / A;

				float energy = 1.0 - subRadius*subRadius;
				energy = energy*energy*energy;
				float temp = energy;
				energy -= previousEnergy;
				previousEnergy = temp;

				intersectionList.push_back(pair<float,float>(near_t, energy));
				intersectionList.push_back(pair<float,float>(far_t, -energy));
			}
		}
	}

	if(intersectionList.empty())
		return false;

	sort(intersectionList.begin(),intersectionList.end());

	vector<pair<float,float> >::iterator inter;

	float energySum = 0.0;
	for(inter=intersectionList.begin(); inter!=intersectionList.end(); inter++)
	{
		energySum += inter->second;
		if(energySum > 0.5)
		{
			t = inter->first;
			p = rt.e() + rt.d()*t;
			n = normal(p);
			p = _transform.mult(p,1.0);
			n = _transform.inverse().transpose().mult(n).normalized();
			u = 0.0;
			v = 0.0;

			return true;
		}
	}

	return false;
}


Vector Metaballs::normal(const Vector &p)
{
	Vector gradient;
	vector<pair<Vector,float> >::iterator it;
	for(it=balls.begin(); it!=balls.end(); it++)
	{
		Vector normal = p - it->first;
		float fDistSquare = normal.dot(normal);
		if(fDistSquare < eps)
			continue;

		float fDistFour = fDistSquare*fDistSquare;
		normal = normal * ((it->second*it->second)/fDistFour);

		gradient = gradient + normal;
	}
	gradient = gradient.normalized();
	return gradient;
}
