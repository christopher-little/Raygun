#pragma once

#ifndef SCENE
#define SCENE

#include <vector>
#include <map>
#include <string>

#include "MDL\mdl.H"
#include "Vector.h"
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
	void addShape(Shape *shape) { shapeList.push_back(shape); }
	void addLight(Light *light) { lightList.push_back(light); }

	// Report number of objects and retrieve a specific object
	inline int nShapes() const { return shapeList.size(); }
	inline Shape* getShape(int i) { return shapeList[i]; }

	inline int nLights() const { return lightList.size(); }
	inline Light* getLight(int i) { return lightList[i]; }

private:
	std::map<std::string,Material*> matMap; // Map material name to matList index
	std::vector<Shape*> shapeList;	// List of shapes to render
	std::vector<Light*> lightList;	// List of light sources (these can correspond to a shape object)
};

#endif