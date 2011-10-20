#pragma once

#ifndef SCENE
#define SCENE

#include <vector>
#include <map>
#include <string>

#include "MDL\mdl.H"
#include "Vector.h"
#include "Colour.h"
#include "Shape.h"
#include "Light.h"
#include "Camera.h"
#include "Material.h"

class Scene
{
public:
	Scene();
	~Scene();

	// Build a scene and return the Camera object to the ray tracer
	Camera* loadMDL(char *filename);


	// Add objects to the scene
	inline void addShape(Shape *shape) { shapeList.push_back(shape); }
	inline void addLight(Light *light) { lightList.push_back(light); }

	// Report number of objects and retrieve a specific object
	inline int nShapes() const { return shapeList.size(); }
	inline Shape* getShape(int i) { return shapeList[i]; }

	inline int nLights() const { return lightList.size(); }
	inline Light* getLight(int i) { return lightList[i]; }

	

	// Return true if the scene has a skybox defined
	inline bool skyBox() { return _skyBoxTex[0] != NULL; }

	// Assign texture for given skybox direction (recall -x:0, +x:1, -y:2, +y:3, -z:4, +z:5)
	inline void setSkyBoxTex(int direction, float *texBuffer, int texWidth, int texHeight)
	{
		_skyBoxTex[direction] = texBuffer;

		_skyTexWidth = texWidth;
		_skyTexHeight = texHeight;
	}
	// Sample texture at given uv coordinates and return resulting colour
	inline Colour getSkyBoxTex(int texIndex, float u, float v)
	{
		// Double check there is a valid texture
		if (_skyBoxTex[texIndex] == NULL)
			return Colour();

		// Convert uv coordinate (0.0-1.0), to texture "texel" coordinate and return sampled colour
		int uI = static_cast<int>(u*_skyTexWidth);
		int vI = static_cast<int>(v*_skyTexHeight);
		int i = (vI*_skyTexWidth + uI)*3;
		float *sampleTex = _skyBoxTex[texIndex];
		return Colour(sampleTex[i], sampleTex[i+1], sampleTex[i+2]);
	}

private:
	std::map<std::string,Material*> matMap; // Map material name to matList index
	std::vector<Shape*> shapeList;	// List of shapes to render
	std::vector<Light*> lightList;	// List of light sources (these can correspond to a shape object)

	float *_skyBoxTex[6];				// Six skybox textures (-x, +x, -y, +y, -z, +z)
	int _skyTexWidth, _skyTexHeight;	// Width and height of skybox textures (Assuming all 6 are the same size)
};

#endif