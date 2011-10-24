#pragma once

#ifndef SCENE
#define SCENE

#include <vector>
#include <map>
#include <string>

#include "MDL\mdl.H"
#include "ImageBuffer.h"
#include "Vector.h"
#include "Colour.h"
#include "Shape.h"
#include "Light.h"
#include "Camera.h"
#include "Material.h"

#include "TRACE.h"

class Scene
{
public:
	Scene();
	~Scene();


	// Add objects to the scene
	inline void addShape(Shape *shape) { shapeList.push_back(shape); }
	inline void addLight(Light *light) { lightList.push_back(light); }

	// Report number of objects and retrieve a specific object
	inline int nShapes() const { return shapeList.size(); }
	inline Shape* getShape(int i) { return shapeList[i]; }

	inline int nLights() const { return lightList.size(); }
	inline Light* getLight(int i) { return lightList[i]; }

	// Temporary function to  give access to material objects
	inline Material *getMat(std::string mtrlName)
	{
		std::map<std::string, Material*>::iterator it = matMap.find(mtrlName);
		if(it != matMap.end())
			return it->second;
		else
			return NULL;
	}


	

	// Build a scene and return the Camera object to the ray tracer
	Camera* loadMDL(char *filename);



private:
	std::map<std::string,Material*> matMap; // Map material name to Material object
	std::map<std::string,ImageBuffer*> texMap; // Map texture name to ImageBuffer object containing the texture
	std::vector<Shape*> shapeList;	// List of shapes to render
	std::vector<Light*> lightList;	// List of light sources (these can correspond to a shape object)

	//ImageBuffer *_skyBoxTex[6];				// Six skybox textures (-x, +x, -y, +y, -z, +z)
};

#endif