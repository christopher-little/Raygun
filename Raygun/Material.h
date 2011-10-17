#pragma once

#ifndef MATERIAL
#define MATERIAL

#include "Colour.h"

class Material
{
public:
	// Default material to a grey colour
	Material()
	{
		_a = Colour(0.1f, 0.1f, 0.1f);
		_d = Colour(0.6f, 0.6f, 0.6f);
		_s = Colour(0.9f, 0.9f, 0.9f);
		_p = 1.0;
		_dielectric = false;
	}
	~Material() {}


	// Retreive Material properties
	inline Colour a() const { return _a; }
	inline Colour d() const { return _d; }
	inline Colour s() const { return _s; }
	inline float  p() const { return _p; }
	inline bool   isDielectric() const { return _dielectric; }
	inline float  n() const { return _n; }


	// Create a "lambertian" diffuse only material
	void makeLambertian(Colour &d)
	{
		_a = Colour();
		_d = d;
		_s = Colour();
		_p = 1.0;
	}

	// Create a "Phong" material
	void makePhong(Colour &d, Colour &s, float p)
	{
		_a = Colour();
		_d = d;
		_s = s;
		_p = p;
	}
	// Create a "Phong" material with an ambient term
	void makePhongAmb(Colour &a, Colour &d, Colour &s, float p)
	{
		makePhong(d, s, p);
		_a = a;
	}
	// Create a Phong dielectric material (no ambient or diffuse, just specular reflection and transmission)
	void makePhongDielectric(Colour &s, float p, float n)
	{
		// In this case, 's' is meant to be "transmittance per metre" rather than direct colour, thus thinner object results in less intense colour.
		makePhong(Colour(), s, p);
		_dielectric = true;
		_n = n;
	}

private:
	Colour _a; // ambient
	Colour _d; // diffuse
	Colour _s; // specular
	float  _p; // Phong exponent
	bool   _dielectric; // Specifies if the material is a dielectric (transmissive) material
	float  _n; // Index of refraction
};

#endif