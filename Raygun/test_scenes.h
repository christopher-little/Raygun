#pragma once

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
#include "Metaballs.h"
#include "Light.h"
#include "PointLight.h"

namespace testscene {

static Scene *test1()
{
	Scene *scene = new Scene();
	
	// Create the camera object
	Vector e(1.0f,14.0f,10.0f);
	Vector g = Vector(0.2f,-0.3f,-1.0f).normalized();
	Vector t = Vector(0.0f,1.0f,0.0f).normalized();
	float N = 2.2;
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
	// Less intense reflective white material
	mat = new Material();
	mat->makePhong(	Colour(0.6f,0.6f,0.6f),
									Colour(0.3f,0.3f,0.3f),
									256.0f);
	scene->setMat(string("glossy white"), mat);
	// Orange dielectric
	mat = new Material();
	mat->makePhongDielectric(	Colour(1.0f,0.5f,0.2f),
														256.0f, 2.0f );
	scene->setMat(string("orange glass"), mat);
	// earth material
	mat = new Material();
	mat->makeLambertian( Colour(1.0f,1.0f,1.0f) );
	mat->setTexture(scene->getTex("cloud earth"));
	scene->setMat(string("earth"), mat);



	// Set a sky box
	scene->setSkyBox(	readJPG((char*)"..\\textures\\skybox_space\\space-x.jpg"),
										readJPG((char*)"..\\textures\\skybox_space\\space+x.jpg"),
										readJPG((char*)"..\\textures\\skybox_space\\space-y.jpg"),
										readJPG((char*)"..\\textures\\skybox_space\\space+y.jpg"),
										readJPG((char*)"..\\textures\\skybox_space\\space-z.jpg"),
										readJPG((char*)"..\\textures\\skybox_space\\space+z.jpg") );




	// Square platform
	Mesh *msh = new Mesh();
	msh->addVertex(Vector(-20.0f,0.0f,-50.0f));
	msh->addVertex(Vector(-20.0f,0.0f,0.0f));
	msh->addVertex(Vector(30.0f,0.0f,0.0f));
	msh->addVertex(Vector(30.0f,0.0f,-50.0f));

	msh->addUVCoord(0.0f,1.0f);
	msh->addUVCoord(0.0f,0.0f);
	msh->addUVCoord(1.0f,0.0f);
	msh->addUVCoord(1.0f,1.0f);

	msh->setVertsPerFace(3);
	
	msh->addFacePoint(0);
	msh->addFacePoint(1);
	msh->addFacePoint(2);
	
	msh->addFacePoint(0);
	msh->addFacePoint(2);
	msh->addFacePoint(3);
	msh->setMat(scene->getMat("glossy white"));
	scene->addShape(msh);

	// Diamond
	msh = new Mesh();
	msh->addVertex(Vector(-3.0f,14.0f,-4.0f));
	msh->addVertex(Vector(-3.0f,0.0f,-4.0f));
	msh->addVertex(Vector(-6.0f,7.0f,-4.0f));
	msh->addVertex(Vector(-6.0f,7.0f,-4.0f));
	msh->addVertex(Vector(0.0f,7.0f,-4.0f));
	msh->addVertex(Vector(-3.0f,7.0f,-1.0f));
	msh->addVertex(Vector(-3.0f,7.0f,-7.0f));

	msh->addUVCoord(0.5f,1.0f);
	msh->addUVCoord(0.5f,0.0f);
	msh->addUVCoord(0.0f,0.5f);
	msh->addUVCoord(1.0f,0.5f);
	msh->addUVCoord(0.5f,0.5f);
	msh->addUVCoord(0.25f,0.5f);
	msh->addUVCoord(0.75f,0.5f);

	msh->setVertsPerFace(3);
	
	msh->addFacePoint(0);
	msh->addFacePoint(2);
	msh->addFacePoint(5);
	
	msh->addFacePoint(0);
	msh->addFacePoint(5);
	msh->addFacePoint(4);
	
	msh->addFacePoint(5);
	msh->addFacePoint(2);
	msh->addFacePoint(1);
	
	msh->addFacePoint(5);
	msh->addFacePoint(1);
	msh->addFacePoint(4);
	
	msh->addFacePoint(0);
	msh->addFacePoint(4);
	msh->addFacePoint(6);
	
	msh->addFacePoint(0);
	msh->addFacePoint(6);
	msh->addFacePoint(3);
	
	msh->addFacePoint(6);
	msh->addFacePoint(4);
	msh->addFacePoint(1);
	
	msh->addFacePoint(6);
	msh->addFacePoint(1);
	msh->addFacePoint(3);
	msh->setMat(scene->getMat("glossy white"));
	scene->addShape(msh);



	

	Shape *shp = new Sphere(	Vector(5.0f,3.5f,-6.0f), 3.0f );
	shp->setMat(scene->getMat("orange glass"));
	scene->addShape(shp);
	
	shp = new Sphere(	Vector(14.0f,3.0f,-4.0f), 3.0f );
	shp->setMat(scene->getMat("light white"));
	scene->addShape(shp);


	shp = new Sphere(	Vector(20.0f,15.0f,-25.0f), 10.0f );
	shp->setMat(scene->getMat("earth"));
	scene->addShape(shp);


	return scene;
}





static Scene *metaballtest()
{
	Scene *scene = new Scene();
	
	// Camera
	Vector e(0.0f,0.0f,20.0f);
	Vector g = Vector(0.0f,0.0f,-1.0f).normalized();
	Vector t = Vector(0.0f,1.0f,0.0f).normalized();
	float N = 8.0f;
	float w = 16.0f;
	float h = 9.0f;
	scene->setCam( new Camera(e, g, t, N, w, h) );


	
	// Lights
	scene->addLight(	new PointLight(	Vector(15.0f,100.0f,0.0f),
																		Colour(1.0f,1.0f,1.0f) ) );



	// Textures
	ImageBuffer *tex = new ImageBuffer(256,256);
	tex->generatePerlin(3,2);
	scene->setTex("perlin tex", tex);

	// Materials
	Material *mat = new Material();
	mat->makePhongAmb(	Colour(0.1f,0.0f,0.0f),
											Colour(0.8f,0.3f,0.5f),
											Colour(0.6f,0.6f,0.6f),
											256.0f);
	scene->setMat(string("shiny pink"), mat);
	mat = new Material();
	mat->makePhong(	Colour(0.6f,0.6f,0.6f),
									Colour(0.3f,0.3f,0.3f),
									256.0f);
	scene->setMat(string("glossy white"), mat);
	mat = new Material();
	mat->makePhongAmb( Colour(1.0f,1.0f,1.0f),
										 Colour(),
										 Colour(),
										 256.0f);
	mat->setTexture(scene->getTex("perlin tex"));
	scene->setMat(string("perlin"), mat);


	
	// Skybox
	scene->setSkyBox(	readJPG((char*)"..\\textures\\skybox_sun\\box-x.jpg"),
										readJPG((char*)"..\\textures\\skybox_sun\\box+x.jpg"),
										readJPG((char*)"..\\textures\\skybox_sun\\box-y.jpg"),
										readJPG((char*)"..\\textures\\skybox_sun\\box+y.jpg"),
										readJPG((char*)"..\\textures\\skybox_sun\\box-z.jpg"),
										readJPG((char*)"..\\textures\\skybox_sun\\box+z.jpg") );



	
	// Shapes
	Mesh *msh = new Mesh();
	msh->addVertex(Vector(-40.0f,-20.0f,-40.0f));
	msh->addVertex(Vector(-40.0f,-20.0f,40.0f));
	msh->addVertex(Vector(40.0f,-20.0f,40.0f));
	msh->addVertex(Vector(40.0f,-20.0f,-40.0f));

	msh->addUVCoord(0.0f,1.0f);
	msh->addUVCoord(0.0f,0.0f);
	msh->addUVCoord(1.0f,0.0f);
	msh->addUVCoord(1.0f,1.0f);

	msh->setVertsPerFace(3);
	
	msh->addFacePoint(0);
	msh->addFacePoint(1);
	msh->addFacePoint(2);
	
	msh->addFacePoint(0);
	msh->addFacePoint(2);
	msh->addFacePoint(3);
	msh->setMat(scene->getMat("glossy white"));
	scene->addShape(msh);

	
	msh = new Mesh();
	msh->addVertex(Vector(-20.0f,20.0f,-30.0f));
	msh->addVertex(Vector(-20.0f,-20.0f,-30.0f));
	msh->addVertex(Vector(20.0f,-20.0f,-30.0f));
	msh->addVertex(Vector(20.0f,20.0f,-30.0f));

	msh->addUVCoord(0.0f,1.0f);
	msh->addUVCoord(0.0f,0.0f);
	msh->addUVCoord(1.0f,0.0f);
	msh->addUVCoord(1.0f,1.0f);

	msh->setVertsPerFace(3);
	
	msh->addFacePoint(0);
	msh->addFacePoint(1);
	msh->addFacePoint(2);
	
	msh->addFacePoint(0);
	msh->addFacePoint(2);
	msh->addFacePoint(3);
	msh->setMat(scene->getMat("perlin"));
	scene->addShape(msh);


	/*
	Shape *shp = new Sphere(	Vector(0.0f,0.0f,0.0f), 5.0f );
	shp->setMat(scene->getMat("shiny pink"));
	scene->addShape(shp);
	*/

	Metaballs *m = new Metaballs();
	m->addSphere( Vector(0.0f,0.0f,0.0f), 5.0f );
	m->addSphere( Vector(8.0f,0.0f,0.0f), 5.0f );
	m->setMat(scene->getMat("shiny pink"));
	scene->addShape(m);

	return scene;
}

}

#endif