#pragma once

#ifndef SHADER
#define SHADER

#include "Vector.h"
#include "Colour.h"
#include "Material.h"

// Shader object which takes Material, Light and ray intersection inputs and produces a colour value.
// NOTE: This is a RAY colour shader only (rather than the analogous pixel shader), not a vertex shader.

class Shader {
public:
	Shader() {}
	~Shader() {}

	// The static pre-made phong shader
    static Colour phong(Material *mat, // Surface material properties
                        Colour &amb, Colour &diffIllum, Colour &specIllum, Colour &specRefl, Colour &specRefr, // Incoming light intensities (***Is ambient needed?)
                        float fresnelR, float ambOccl, // Fresnel reflection/refraction amount, ambient occlusion value
                        Vector &p, Vector &n, float u, float v ) // Surface intersection point, normal vector and u,v texture coordinates
	{
		Colour texColour(1.0f,1.0f,1.0f);
		if(mat->hasTexture())
		{
			texColour = mat->sampleTex(u, v);
		}

		Colour returnColour =	( amb*mat->a() + diffIllum*mat->d() )*texColour + // Ambient and diffuse illumination
                                ( specIllum + specRefl*fresnelR + specRefr*(1-fresnelR) )*mat->s(); // specular illumination, reflection, refraction
		return returnColour.clamp() * ambOccl;
	}
};

#endif
