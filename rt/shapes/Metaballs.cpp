#include "Metaballs.h"

#include <vector>
#include <algorithm>

using namespace std;

const float eps = 0.0001f;


bool Metaballs::intersect(const Ray &r, float &t, Vector &p, Vector &n, float &u, float &v)
{
	if(balls.empty())
		return false;

	// Transform the ray to the local transformation.
	Ray rt = _transform.inverse() * r;


	// This list contains pairs of distance and energy level.
	vector<pair<float,float> > intersectionList;

	// Iterate through each 'ball' by intersecting the sphere surrounding each
	// point.
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

		// When an intersection is found, begin intersecting interior concentric
		// spheres. At each concentric intersection, find distance (t) to enter
		// and exit. At entry, energy is added, at exit, energy is subtracted,
		// thus energy is added to intersectionList and then subtracted.
		float sqrtDiscrim, near_t, far_t, tempRadius;
		sqrtDiscrim = sqrt(discrim);
		//near_t = (-B - sqrtDiscrim) / A;
		far_t = (-B + sqrtDiscrim) / A;
		if(far_t > eps)
		{
			float previousEnergy = 0.0;
			for(float subRadius=0.95; subRadius > 0.1; subRadius-=0.01)
			{
				tempRadius = subRadius*radius;
				C = e_c.dot(e_c) - tempRadius*tempRadius;

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

	// Bail out early when there is no energy added.
	if(intersectionList.empty())
		return false;

	// Sort the list by distance to concentric sphere intersection
	sort(intersectionList.begin(),intersectionList.end());

	// Iterate through the sorted list of intersections and sum the energy. If
	// the energy reaches the minimum threshold then an intersection occurs.
	vector<pair<float,float> >::iterator inter;
	float energySum = 0.0;
	for(inter=intersectionList.begin(); inter!=intersectionList.end(); inter++)
	{
		energySum += inter->second;
		if(energySum > _threshold)
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
	// Sum normals at p for each ball centre and then normalize.
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
