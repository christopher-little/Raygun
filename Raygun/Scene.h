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



	// Get/search the for a material
	inline Material *getMat(std::string mtrlName)
	{
		std::map<std::string, Material*>::iterator it = matMap.find(mtrlName);
		if(it != matMap.end())
			return it->second;
		else
		{
			TRACE("Scene: Warning: Material with name \"%s\" could not be found.\n", mtrlName.c_str());
			return NULL;
		}
	}
	// Map a material name to a material
	inline bool setMat(std::string mtrlName, Material *mat)
	{
		std::map<std::string, Material*>::iterator it = matMap.find(mtrlName);
		if(it == matMap.end())
		{
			matMap.insert(std::pair<std::string,Material*>(mtrlName,mat));
			return true;
		}
		else
		{
			TRACE("Scene: Warning: Material with name \"%s\" already exists. Proceeding with previous material.\n", mtrlName.c_str());
			return false;
		}
	}

	
	// Get/search the for a texture
	inline ImageBuffer *getTex(std::string txtrName)
	{
		std::map<std::string, ImageBuffer*>::iterator it = texMap.find(txtrName);
		if(it != texMap.end())
			return it->second;
		else
		{
			TRACE("Scene: Warning: Texture with name \"%s\" could not be found.\n", txtrName.c_str());
			return NULL;
		}
	}
	// Map a material name to a material
	inline bool setTex(std::string txtrName, ImageBuffer *tex)
	{
		std::map<std::string, ImageBuffer*>::iterator it = texMap.find(txtrName);
		if(it == texMap.end())
		{
			texMap.insert(std::pair<std::string,ImageBuffer*>(txtrName,tex));
			return true;
		}
		else
		{
			TRACE("Scene: Warning: Texture with name \"%s\" already exists. Proceeding with previous material.\n", txtrName.c_str());
			return false;
		}
	}


	

	// Retrieve the camera object
	inline Camera *cam() { return _cam; }
	// Set the camera for the scene
	inline void setCam( Camera *cam )
	{
		if( _cam != NULL )
		{
			TRACE("Scene: Warning: Camera object already exists; replacing with new camera.\n");
			delete _cam;
		}
		_cam = cam;
	}




	// Set the skyboxes given six ImageBuffer pointers
	inline bool setSkyBox(	ImageBuffer *x0, ImageBuffer *x1,
													ImageBuffer *y0, ImageBuffer *y1,
													ImageBuffer *z0, ImageBuffer *z1 )
	{
		_skyBox[0] = x0;
		_skyBox[1] = x1;
		_skyBox[2] = y0;
		_skyBox[3] = y1;
		_skyBox[4] = z0;
		_skyBox[5] = z1;

		if( x0==NULL || x1==NULL || y0==NULL || y1==NULL || z0==NULL || z1==NULL )
		{
			TRACE("Scene: Warning: A skybox ImageBuffer is NULL, this will likely cause a crash\n");
			return false;
		}

		return true;
	}
	// Get a skybox texture
	inline ImageBuffer *getSkyBox(int i) { return _skyBox[i]; }


	

	// Build a scene and return the Camera object to the ray tracer
	void loadMDL(char *filename);



private:
	std::vector<Shape*> shapeList;	// List of shapes to render
	std::vector<Light*> lightList;	// List of light sources (these can correspond to a shape object)

	std::map<std::string,Material*> matMap; // Map material name to Material object
	std::map<std::string,ImageBuffer*> texMap; // Map texture name to ImageBuffer object containing the texture

	ImageBuffer *_skyBox[6];				// Six skybox textures (-x, +x, -y, +y, -z, +z)

	Camera *_cam;
};

#endif