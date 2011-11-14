#include "Scene.h"

#include "ImageIO.h"
#include "Sphere.h"
#include "Light.h"
#include "PointLight.h"
#include "Mesh.h"

Scene::Scene()
{
	// Ensure a clean Scene has all object members initialized to NULL

	for(int i=0; i<6; i++)
		_skyBox[i] = NULL;

	_cam = NULL;
}


Scene::~Scene()
{
	// Clear out all lists and objects

	shapeList.clear();
	lightList.clear();
	texMap.clear();
	matMap.clear();

	for(int i=0; i<6; i++)
		delete _skyBox[i];

	delete _cam;
}
