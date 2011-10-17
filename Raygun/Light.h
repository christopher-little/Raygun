#pragma once

#ifndef LIGHT
#define LIGHT

class Light
{
public:
	Light(Vector &p, Colour &c) : _p(p), _c(c)
	{

	}
	~Light(void) {}

	inline Vector p() const { return _p; }
	inline Colour c() const { return _c; }

private:
	Vector _p;	// Point light position
	Colour _c;	// Light emittance colour
};

#endif