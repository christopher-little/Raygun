#pragma once

#ifndef COLOUR
#define COLOUR

#ifdef _WIN32
#include <Windows.h>
#endif

#include <vector>

////////////////////////////////////////////////////////////////////////////////
// class Colour
//
// Container for a rgb colour value/pixel
////////////////////////////////////////////////////////////////////////////////
class Colour
{
public:
	Colour() : _r(0.0), _g(0.0), _b(0.0) {}
	Colour(float inr, float ing, float inb) : _r(inr), _g(ing), _b(inb) {}
	~Colour(void) {}

	// Return float value of component
	inline float r() const { return _r; }
	inline float g() const { return _g; }
	inline float b() const { return _b; }
	// Return integer value (0-255) of component
	inline int r_int() const { return static_cast<int>(_r*255); }
	inline int g_int() const { return static_cast<int>(_g*255); }
	inline int b_int() const { return static_cast<int>(_b*255); }
#ifdef _WIN32
	// Return a COLORREF RGB value for windows
	inline COLORREF getWinRGB() const { return RGB(r_int(),g_int(),b_int()); }
#endif
	
	// Sum colour components
	inline Colour operator+ (Colour c) const { return Colour(_r+c._r, _g+c._g, _b+c._b); }
	// Multiply colour components by scalar
	inline Colour operator* (float m) const { return Colour(_r*m, _g*m, _b*m); }
	// Multiply two colours (i.e. multiply material colour by light colour)
	inline Colour operator* (Colour c) const { return Colour(_r*c._r, _g*c._g, _b*c._b); }
	// Divide colour components by scalar
	inline Colour operator/ (float m) const { if(m > 0.0f) { return Colour(_r/m, _g/m, _b/m); } else { return Colour(1.0,1.0,1.0); } }

	// Invert the colour (1-colour)
	inline void invert()
	{
		_r = 1.0f-_r;
		_g = 1.0f-_g;
		_b = 1.0f-_b;
	}

	// Clamp the components to 0.0 .. 1.0
	inline Colour clamp() const
	{
		Colour out(_r,_g,_b);
		if(_r < 0.0)
			out._r = 0.0;
		if(_r > 1.0)
			out._r = 1.0;
		if(_g < 0.0)
			out._g = 0.0;
		if(_g > 1.0)
			out._g = 1.0;
		if(_b < 0.0)
			out._b = 0.0;
		if(_b > 1.0)
			out._b = 1.0;

		return out;
	}

	// Return true if any component is larger than 0.0
	bool visible()
	{
		if(_r > 0.0f)
			return true;
		if(_g > 0.0f)
			return true;
		if(_b > 0.0f)
			return true;

		return false;
	}


	// Convert Vector to char* for printing, etc.
	operator char *() const
	{
		char *vecString = new char[128];
		sprintf(vecString,"(%f, %f, %f)", _r, _g, _b);
		return vecString;
	}
private:
	float _r,_g,_b;	// red, green and blue colour components
};

#endif