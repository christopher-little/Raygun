#ifndef _TEST_SCENES_
#define _TEST_SCENES_

#include <string>

using namespace std;

#include "Scene.h"
#include "ImageIO.h"
#include "Material.h"
#include "Shape.h"
#include "Sphere.h"
#include "Mesh.h"
#include "Light.h"
#include "PointLight.h"

namespace testscene {
static Scene *test1()
{
	Scene *scene = new Scene();
	
	// Create the camera object
	Vector e(5.0f,3.0f,10.0f);
	Vector g = Vector(0.0f,-0.2f,-1.0f).normalized();
	Vector t = Vector(0.0f,1.0f,0.0f).normalized();
	float N = 2.0;
	float w = 5.3;
	float h = 3.0;
	scene->setCam( new Camera(e, g, t, N, w, h) );



	// Add some illumination to the situation
	scene->addLight(	new PointLight(	Vector(-100.0f,50.0f,100.0f),
																		Colour(1.0f,1.0f,1.0f) ) );
	scene->addLight(	new PointLight(	Vector(100.0f,150.0f,100.0f),
																		Colour(1.0f,1.0f,1.0f) ) );



	// Load in some textures
	scene->setTex("cloud earth", readJPG("..\\textures\\cloud_earth.jpg") );
	scene->setTex("mars", readJPG("..\\textures\\mars.jpg") );


	// White, slightly reflective
	Material *mat = new Material();
	mat->makePhong(	Colour(0.7f,0.7f,0.7f),
									Colour(0.9f,0.9f,0.9f),
									256.0f);
	scene->setMat(string("light white"), mat);
	mat = new Material();
	mat->makePhong(	Colour(0.5f,0.5f,0.5f),
									Colour(0.5f,0.5f,0.5f),
									256.0f);
	scene->setMat(string("glossy white"), mat);



	// Set a sky box
	scene->setSkyBox(	readJPG((char*)"..\\textures\\skybox_space\\space-x.jpg"),
										readJPG((char*)"..\\textures\\skybox_space\\space+x.jpg"),
										readJPG((char*)"..\\textures\\skybox_space\\space-y.jpg"),
										readJPG((char*)"..\\textures\\skybox_space\\space+y.jpg"),
										readJPG((char*)"..\\textures\\skybox_space\\space-z.jpg"),
										readJPG((char*)"..\\textures\\skybox_space\\space+z.jpg") );



	// Square platform
	Mesh *msh = new Mesh();
	msh->addVertex(Vector(-20.0f,-7.0f,-50.0f));
	msh->addVertex(Vector(-20.0f,-7.0f,0.0f));
	msh->addVertex(Vector(30.0f,-7.0f,0.0f));
	msh->addVertex(Vector(30.0f,-7.0f,-50.0f));
	msh->addUVCoord(0.0f,1.0f);
	msh->addUVCoord(0.0f,0.0f);
	msh->addUVCoord(1.0f,0.0f);
	msh->addUVCoord(1.0f,1.0f);
	msh->setVertsPerFace(3);
	// Face 1
	msh->addFacePoint(0);
	msh->addFacePoint(1);
	msh->addFacePoint(2);
	// Face 2
	msh->addFacePoint(0);
	msh->addFacePoint(2);
	msh->addFacePoint(3);
	msh->setMat(scene->getMat("glossy white"));
	scene->addShape(msh);




	// Make a sphere
	Shape *shp = new Sphere(	Vector(5.0f,-3.5f,-4.0f), 3.0f );
	shp->setMat(scene->getMat("light white"));
	scene->addShape(shp);


	return scene;
}
}

#endif