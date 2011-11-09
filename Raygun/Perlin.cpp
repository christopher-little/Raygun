#include "Perlin.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

#include "Vector.h"

#include "TRACE.h"



Perlin::Perlin(int octaves, int power)
{
	// Each octave is a gradient grid with a different density and amplitude
	_octaves.resize(octaves);

	_basePower = power;
	
	// Use bit shifts to raise power of 2
	int exp = 1 << power;
	// Initialize the gradient array for each octave
	for(int i=0; i<octaves; i++)
	{
		_octaves[i].resize(exp*exp*exp);
		for(int x=0; x<exp; x++) for(int y=0; y<exp; y++) for(int z=0; z<exp; z++)
		{
			int j = x + y*exp + z*exp*exp;

			float denom = 1.0f/((float)RAND_MAX*0.5f);
			_octaves[i][j]   = Vector(((float)rand() *denom)-1.0f, ((float)rand() *denom)-1.0f, ((float)rand() *denom)-1.0f);
		}
		// Double the gradient grid density and half the amplitude for the next octave
		exp = exp << 1;
	}
}





Perlin::~Perlin()
{
	for(int i=0; i<_octaves.size(); i++)
	{
		_octaves[i].clear();
	}

	_octaves.clear();
}





vector<float> *Perlin::noise(float x, float y, float z)
{
	float noiseValue = 0.0f;
	vector<float> *noiseValues = new vector<float>(_octaves.size());
	
	// Dimension size for current octave
	int dim = 1 << _basePower;
	// Amplitude for current octave
	float amp = 1.0f;
	for(int i=0; i<_octaves.size(); i++)
	{
		// Calculate position in gradient grid
		Vector p(x*dim, y*dim, z*dim);
		// Map to absolute grid point
		int pIx = static_cast<int>(p.x());
		int pIy = static_cast<int>(p.y());
		int pIz = static_cast<int>(p.z());
		// Fractional position of point within grid cube
		Vector lp(p.x()-pIx, p.y()-pIy, p.z()-pIz);

		
		// Compute vector from grid corner to point, then compute dot product with gradient vector for that corner
		int j=pIx + pIy*dim + pIz*dim*dim;
		Vector temp = Vector(lp-Vector(0.0f,0.0f,0.0f));
		float a = _octaves[i][j].dot(temp);

		temp = Vector(lp-Vector(1.0f,0.0f,0.0f));
		float b = _octaves[i][j+1].dot(temp);

		j=pIx + (pIy+1)*dim + pIz*dim*dim;
		temp = Vector(lp-Vector(0.0f,1.0f,0.0f));
		float c = _octaves[i][j].dot(temp);
		
		temp = Vector(lp-Vector(1.0f,1.0f,0.0f));
		float d = _octaves[i][j+1].dot(temp);

		j=pIx + pIy*dim + (pIz+1)*dim*dim;
		temp = Vector(lp-Vector(0.0f,0.0f,1.0f));
		float e = _octaves[i][j].dot(temp);
		
		temp = Vector(lp-Vector(1.0f,0.0f,1.0f));
		float f = _octaves[i][j+1].dot(temp);

		j=pIx + (pIy+1)*dim + (pIz+1)*dim*dim;
		temp = Vector(lp-Vector(0.0f,1.0f,1.0f));
		float g = _octaves[i][j].dot(temp);
		
		temp = Vector(lp-Vector(1.0f,1.0f,1.0f));
		float h = _octaves[i][j+1].dot(temp);

		
		// Weight p in the x,y,z directions using S-curve
		float sx = 3*(lp.x()*lp.x()) - 2*(lp.x()*lp.x()*lp.x());
		float sy = 3*(lp.y()*lp.y()) - 2*(lp.y()*lp.y()*lp.y());
		float sz = 3*(lp.z()*lp.z()) - 2*(lp.z()*lp.z()*lp.z());

		// Interpolate in x direction
		float xa = a + sx*(b-a);
		float xb = c + sx*(d-c);
		float xc = e + sx*(f-e);
		float xd = g + sx*(h-g);
		// Interpolate in y direction
		float ya = xa + sy*(xb-xa);
		float yb = xc + sy*(xd-xc);
		// Interpolate in z direction
		float za = ya + sz*(yb-ya);

		noiseValue += za*amp;
		noiseValues->push_back(za*amp);

		dim = dim << 1;
		amp *= 0.5f;
	}

	if(noiseValue > 1.0f)
		noiseValue = 1.0f;
	else if(noiseValue < -1.0f)
		noiseValue = -1.0f;
	
	//return noiseValue;
	return noiseValues;
}
