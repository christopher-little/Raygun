#pragma once

#ifndef SCENE
#define SCENE

#include <vector>
#include <map>
#include <string>
#include <iostream>

#include "ImageBuffer.h"
#include "Vector.h"
#include "Colour.h"
#include "Shape.h"
#include "Light.h"
#include "Camera.h"
#include "Material.h"

using namespace std;


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
	inline Material *getMat(string mtrlName)
	{
		map<string, Material*>::iterator it = matMap.find(mtrlName);
		if(it != matMap.end())
			return it->second;
		else
		{
			cout << "Scene: Warning: Material with name \""<< mtrlName << "\" could not be found." << endl;
			return NULL;
		}
	}
	// Map a material name to a material
	inline bool setMat(string mtrlName, Material *mat)
	{
		map<string, Material*>::iterator it = matMap.find(mtrlName);
		if(it == matMap.end())
		{
			matMap.insert(pair<string,Material*>(mtrlName,mat));
			return true;
		}
		else
		{
			cout << "Scene: Warning: Material with name \"" << mtrlName << "\" already exists. Proceeding with previous material." << endl;
			return false;
		}
	}

	
	// Get/search the for a texture
	inline ImageBuffer *getTex(string txtrName)
	{
		map<string, ImageBuffer*>::iterator it = texMap.find(txtrName);
		if(it != texMap.end())
			return it->second;
		else
		{
			cout << "Scene: Warning: Texture with name \"" << txtrName << "\" could not be found." << endl;
			return NULL;
		}
	}
	// Map a material name to a material
	inline bool setTex(string txtrName, ImageBuffer *tex)
	{
		map<string, ImageBuffer*>::iterator it = texMap.find(txtrName);
		if(it == texMap.end())
		{
			texMap.insert(pair<string,ImageBuffer*>(txtrName,tex));
			return true;
		}
		else
		{
			cout << "Scene: Warning: Texture with name \"" << txtrName << "\" already exists. Proceeding with previous material." << endl;
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
			cout << "Scene: Warning: Camera object already exists; replacing with new camera." << endl;
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
			cout << "Scene: Warning: A skybox ImageBuffer is NULL, this will likely cause a crash." << endl;
			return false;
		}

		return true;
	}
	// Get a skybox texture
	inline ImageBuffer *getSkyBox(int i) { return _skyBox[i]; }



private:
	vector<Shape*> shapeList;	// List of shapes to render
	vector<Light*> lightList;	// List of light sources (these can correspond to a shape object)

	map<string,Material*> matMap; // Map material name to Material object
	map<string,ImageBuffer*> texMap; // Map texture name to ImageBuffer object containing the texture

	ImageBuffer *_skyBox[6];				// Six skybox textures (-x, +x, -y, +y, -z, +z)

	Camera *_cam;
};

#endif
