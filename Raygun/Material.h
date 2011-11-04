#pragma once

#ifndef MATERIAL
#define MATERIAL

#include "Colour.h"
#include "ImageBuffer.h"

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

		_texture = NULL;
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
		_a = Colour(0.1, 0.1, 0.1);
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


	// Return true if the Material contains a texture
	inline bool hasTexture() { return _texture != NULL; }
	// Assign the ImageBuffer tex as the texture for this material, returns true if the texture is successfully set
	bool setTexture(ImageBuffer *tex)
	{
		// Return false if the texture is no good
		if(tex == NULL)
			return false;

		_texture = tex;
		return true;
	}
	// Retrieve a colour value
	Colour sampleTex(float u, float v)
	{
		float eps = 0.001f;

		if(u > 1.0f-eps)
			u = 1.0f-eps;
		if(v > 1.0f-eps)
			v = 1.0f-eps;

		// Compute texture position, texture texel position, and fractional distance inside texel
		float uT = u*(_texture->width()-1);
		float vT = v*(_texture->height()-1);
		int uI = static_cast<int>(uT);
		int vI = static_cast<int>(vT);
		float uD = uT-uI;
		float vD = vT-vI;

		// Bilinear interpolation across the 4 nearest texels
		Colour c0 = _texture->getPixel(vI, uI);
		Colour c1 = _texture->getPixel(vI, uI+1);
		Colour c2 = _texture->getPixel(vI+1, uI);
		Colour c3 = _texture->getPixel(vI+1, uI+1);

		Colour d0 = c0 + (c1*uD + c0*-uD);
		Colour d1 = c2 + (c3*uD + c2*-uD);

		Colour d3 = d0 + (d1*vD + d0*-vD);

		return d3;
	}

private:
	Colour _a; // ambient
	Colour _d; // diffuse
	Colour _s; // specular
	float  _p; // Phong exponent
	bool   _dielectric; // Specifies if the material is a dielectric (transmissive) material
	float  _n; // Index of refraction

	ImageBuffer *_texture; // Pointer to image buffer that contains a texture for the material. When a texture is present, _d diffuse colour becomes modifier for texture colour
};

#endif