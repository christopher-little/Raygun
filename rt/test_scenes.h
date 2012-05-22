//==============================================================================
// Set of functions to generate test scenes
//
// Author: Chris Little
//==============================================================================

#pragma once

#ifndef _TEST_SCENES_
#define _TEST_SCENES_

#include <string>
#include <iostream>

using namespace std;

#include "Scene.h"
#include "ImageIO.h"
#include "Material.h"
#include "shapes/all_shapes.h"
#include "lights/all_lights.h"

#include "Matrix.h"

#include "Perlin.h"


namespace testscene {


static Scene *test1()
{
    Scene *scene = new Scene();

    // Create the camera object
    Vector e(1.0f,14.0f,10.0f);
    Vector g = Vector(0.2f,-0.3f,-1.0f).normalized();
    Vector t = Vector(0.0f,1.0f,0.0f).normalized();
    float N = 2.2f;
    float w = 5.3f;
    float h = 3.0f;
    scene->setCam( new Camera(e, g, t, N, w, h) );



    // Add some illumination to the situation
    scene->addLight( new PointLight( Vector(-100.0f,50.0f,100.0f), Colour(1.0f,1.0f,1.0f) ) );
    scene->addLight( new PointLight( Vector(100.0f,150.0f,100.0f), Colour(1.0f,1.0f,1.0f) ) );



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
    mat->makePhongDielectric( Colour(1.0f,0.5f,0.2f), 256.0f, 2.0f );
    scene->setMat(string("orange glass"), mat);
    // earth material
    mat = new Material();
    mat->makeLambertian( Colour(1.0f,1.0f,1.0f) );
    mat->setTexture(scene->getTex("cloud earth"));
    scene->setMat(string("earth"), mat);



    // Set a sky box
//	scene->setSkyBox( readJPG((char*)"..\\textures\\skybox_space\\space-x.jpg"),
//					  readJPG((char*)"..\\textures\\skybox_space\\space+x.jpg"),
//					  readJPG((char*)"..\\textures\\skybox_space\\space-y.jpg"),
//					  readJPG((char*)"..\\textures\\skybox_space\\space+y.jpg"),
//					  readJPG((char*)"..\\textures\\skybox_space\\space-z.jpg"),
//					  readJPG((char*)"..\\textures\\skybox_space\\space+z.jpg") );
    scene->setSkyBox( readJPG((char*)"..\\textures\\skybox_sun\\box-x.jpg"),
                      readJPG((char*)"..\\textures\\skybox_sun\\box+x.jpg"),
                      readJPG((char*)"..\\textures\\skybox_sun\\box-y.jpg"),
                      readJPG((char*)"..\\textures\\skybox_sun\\box+y.jpg"),
                      readJPG((char*)"..\\textures\\skybox_sun\\box-z.jpg"),
                      readJPG((char*)"..\\textures\\skybox_sun\\box+z.jpg") );




    Mesh *msh;
    MeshShape *mshShape;
    // Square platform
    msh = new Mesh();
    msh->addVertex(Vector(-20.0f,0.0f,-50.0f));
    msh->addVertex(Vector(-20.0f,0.0f,0.0f));
    msh->addVertex(Vector(30.0f,0.0f,0.0f));
    msh->addVertex(Vector(30.0f,0.0f,-50.0f));
    msh->addUVCoord(0.0f,1.0f);
    msh->addUVCoord(0.0f,0.0f);
    msh->addUVCoord(1.0f,0.0f);
    msh->addUVCoord(1.0f,1.0f);
    msh->addFacePoint(0);
    msh->addFacePoint(1);
    msh->addFacePoint(2);
    msh->addFacePoint(0);
    msh->addFacePoint(2);
    msh->addFacePoint(3);

    mshShape = new MeshShape(msh);
    mshShape->setMat(scene->getMat("glossy white"));
    scene->addShape(mshShape);

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

    mshShape = new MeshShape(msh);
    mshShape->setMat(scene->getMat("glossy white"));
    scene->addShape(mshShape);





    Shape *shp = new Sphere( Vector(5.0f,3.5f,-6.0f), 3.0f );
    shp->setMat(scene->getMat("orange glass"));
    scene->addShape(shp);

    shp = new Sphere( Vector(14.0f,3.0f,-4.0f), 3.0f );
    shp->setMat(scene->getMat("light white"));
    scene->addShape(shp);


    shp = new Sphere( Vector(20.0f,15.0f,-25.0f), 10.0f );
    shp->setMat(scene->getMat("earth"));
    scene->addShape(shp);


    return scene;
}





static Scene *perlinTexturing()
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
    scene->addLight( new PointLight( Vector(15.0f,100.0f,0.0f), Colour(1.0f,1.0f,1.0f) ) );



    // Textures
    Perlin p(1,2);
    ImageBuffer *tex = new ImageBuffer(256,256);
    //tex->generatePerlin(p);
    //tex->generatePerlinFlame(p);
    tex->generatePerlinWood(p);
    writeJPG("..\\perlin.jpg", tex);
    scene->setTex("perlin noise", tex);

    tex = readJPG("..\\textures\\bricks_normal.jpg");
    tex->warpPerlin(p);
    scene->setTex("perlin warp", tex);


    // Materials
    // shiny pink
    Material *mat = new Material();
    mat->makePhongAmb(	Colour(0.1f,0.0f,0.0f),
                        Colour(0.8f,0.3f,0.5f),
                        Colour(0.6f,0.6f,0.6f),
                        256.0f);
    scene->setMat(string("shiny pink"), mat);

    // glossy white
    mat = new Material();
    mat->makePhong(	Colour(0.6f,0.6f,0.6f),
                    Colour(0.3f,0.3f,0.3f),
                    256.0f);
    scene->setMat(string("glossy white"), mat);

    // regular perlin noise
    mat = new Material();
    mat->makePhongAmb( Colour(1.0f,1.0f,1.0f),
                       Colour(),
                       Colour(),
                       256.0f);
    mat->setTexture(scene->getTex("perlin noise"));
    scene->setMat(string("perlin noise"), mat);

    // coloured perlin noise
    mat = new Material();
    mat->makePhongAmb(	Colour(0.51f,0.31f,0.26f),
                        Colour(),//0.9f,0.5f,0.1f),
                        Colour(),//0.1f,0.1f,0.1f),
                        256.0f);
    mat->setTexture(scene->getTex("perlin noise"));
    scene->setMat(string("coloured perlin noise"), mat);

    // perlin warp
    mat = new Material();
    mat->makePhongAmb( Colour(1.0f,1.0f,1.0f),
                       Colour(),
                       Colour(),
                       256.0f);
    mat->setTexture(scene->getTex("perlin warp"));
    scene->setMat(string("perlin warp"), mat);

    // shiny perlin warp
    mat = new Material();
    mat->makePhongAmb(	Colour(0.1f,0.1f,0.1f),
                        Colour(0.9f,0.9f,0.9f),
                        Colour(0.1f,0.1f,0.1f),
                        256.0f);
    mat->setTexture(scene->getTex("perlin warp"));
    scene->setMat(string("shiny perlin warp"), mat);



    // Skybox
    scene->setSkyBox(	readJPG((char*)"..\\textures\\skybox_sun\\box-x.jpg"),
                        readJPG((char*)"..\\textures\\skybox_sun\\box+x.jpg"),
                        readJPG((char*)"..\\textures\\skybox_sun\\box-y.jpg"),
                        readJPG((char*)"..\\textures\\skybox_sun\\box+y.jpg"),
                        readJPG((char*)"..\\textures\\skybox_sun\\box-z.jpg"),
                        readJPG((char*)"..\\textures\\skybox_sun\\box+z.jpg") );




    // Shapes
    /*
    //floor
    MeshShape *msh = new MeshShape();
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

    // left and right panels
    msh = new MeshShape();
    msh->addVertex(Vector(-41.0f,20.0f,-30.0f));
    msh->addVertex(Vector(-41.0f,-20.0f,-30.0f));
    msh->addVertex(Vector(-1.0f,-20.0f,-30.0f));
    msh->addVertex(Vector(-1.0f,20.0f,-30.0f));

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
    msh->setMat(scene->getMat("perlin noise"));
    scene->addShape(msh);



    msh = new MeshShape();
    msh->addVertex(Vector(1.0f,20.0f,-30.0f));
    msh->addVertex(Vector(1.0f,-20.0f,-30.0f));
    msh->addVertex(Vector(41.0f,-20.0f,-30.0f));
    msh->addVertex(Vector(41.0f,20.0f,-30.0f));

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
    msh->setMat(scene->getMat("perlin warp"));
    scene->addShape(msh);
    */


    Shape *shp = new Sphere(	Vector(-15.0f,-12.0f,-19.0f), 8.0f );
    shp->setMat(scene->getMat("coloured perlin noise"));
    scene->addShape(shp);

    shp = new Sphere(	Vector(15.0f,-12.0f,-19.0f), 8.0f );
    shp->setMat(scene->getMat("shiny perlin warp"));
    scene->addShape(shp);

    return scene;
}




static Scene *transformScene()
{
    Scene *scene = new Scene();

    // Camera
    Vector e(-30.0,30.0,60.0);
    Vector g = Vector(0.6,-0.5,-1.0).normalized();
    Vector t = Vector(0.0,1.0,0.0).normalized();
    float N = 16.0;
    float w = 16.0;
    float h = 9.0;
    scene->setCam( new Camera(e, g, t, N, w, h) );



    // Lights
    //scene->addLight( new PointLight( Vector(-100.0f,100.0f,0.0f), Colour(1.0f,0.7f,0.7f) ) );
    scene->addLight( new PointLight( Vector(0.0f,100.0f,100.0f), Colour(1.0f,1.0f,1.0f) ) );



    // Textures
    ImageBuffer *tex = readJPG("../textures/cloud_earth.jpg");
    scene->setTex("earth", tex);

    tex = readJPG("../textures/cracked_earth.jpg");
    scene->setTex("stone path", tex);


    // Materials
    Material *mat;
    mat = new Material();
    mat->makePhongAmb(	Colour(0.1,0.1,0.1),
                        Colour(0.6,0.6,0.6),
                        Colour(0.3,0.3,0.3),
                        256.0);
    scene->setMat(string("glossy white"), mat);

    mat = new Material();
    mat->makePhongAmb(	Colour(0.1,0.1,0.1),
                        Colour(0.7,0.7,0.7),
                        Colour(),
                        1.0);
    scene->setMat(string("flat white"), mat);

    mat = new Material();
    mat->makePhongAmb(	Colour(0.15,0.15,0.15),
                        Colour(0.9,0.9,0.9),
                        Colour(),
                        256.0);
    mat->setTexture(scene->getTex("stone path"));
    scene->setMat(string("ground"), mat);

    mat = new Material();
    mat->makePhongAmb(	Colour(),
                        Colour(0.3,0.0,0.0),
                        Colour(0.8,0.3,0.3),
                        256.0);
    scene->setMat(string("shiny red"), mat);

    mat = new Material();
    mat->makePhongAmb(	Colour(),
                        Colour(0.8,0.0,0.0),
                        Colour(),
                        1.0);
    scene->setMat(string("flat red"), mat);

    mat = new Material();
    mat->makePhongDielectric(	Colour(0.8,0.1,0.1),
                                256.0,
                                1.2);
    scene->setMat(string("dielectric red"), mat);

    mat = new Material();
    mat->makePhongAmb(	Colour(),
                        Colour(0.0,0.0,0.3),
                        Colour(0.3,0.3,0.8),
                        256.0);
    scene->setMat(string("shiny blue"), mat);

    mat = new Material();
    mat->makePhongAmb(	Colour(),
                        Colour(0.0,0.0,0.8),
                        Colour(),
                        1.0);
    scene->setMat(string("flat blue"), mat);

    mat = new Material();
    mat->makePhongDielectric(	Colour(0.3,0.3,1.0),
                                256.0,
                                1.2);
    scene->setMat(string("dielectric blue"), mat);

    mat = new Material();
    mat->makePhongAmb(	Colour(0.1,0.1,0.1),
                        Colour(1.0,1.0,1.0),
                        Colour(),
                        1.0);
    mat->setTexture(scene->getTex("earth"));
    scene->setMat(string("earth"), mat);

    mat = new Material();
    mat->makePhongAmb(	Colour(),
                        Colour(0.2,0.9,0.2),
                        Colour(0.1,0.2,0.1),
                        256.0);
    scene->setMat(string("metaball mat"), mat);

    mat = new Material();
    mat->makePhongAmb(	Colour(1.0,1.0,1.0),
                        Colour(),
                        Colour(),
                        1.0);
    scene->setMat(string("light mat"), mat);





    // Objects
    Matrix transform;
    Shape *shape;

    Mesh *squareMesh = Mesh::square();
    Mesh *diamondMesh = Mesh::diamond();

    // Floor
    transform.makeIdentity();
    transform.translate(0.0,0.0,0.0);
    transform.rotate(Vector(1.0,0.0,0.0), 90.0);
    transform.scale(30.0,30.0,1.0);

    shape = new MeshShape(squareMesh);
    shape->setMat(scene->getMat("ground"));
    shape->transform(transform);
    scene->addShape(shape);

    // Walls
    transform.makeIdentity();
    transform.translate(30.0,30.0,0.0);
    transform.rotate(Vector(0.0,1.0,0.0), 90.0);
    transform.scale(30.0,30.0,1.0);
    shape = new MeshShape(squareMesh);
    shape->setMat(scene->getMat("glossy white"));
    shape->transform(transform);
    scene->addShape(shape);

    transform.makeIdentity();
    transform.translate(0.0,30.0,-30.0);
    transform.scale(30.0,30.0,1.0);
    shape = new MeshShape(squareMesh);
    shape->setMat(scene->getMat("glossy white"));
    shape->transform(transform);
    scene->addShape(shape);

    // Ceiling lights
    Matrix lightOrient;
    lightOrient.makeIdentity();
    lightOrient.rotate(Vector(1.0,0.0,0.0), -90.0);
    lightOrient.scale(8.0,8.0,1.0);

    transform.makeIdentity();
    transform.translate(-11.0,60.0,-11.0);
    transform *= lightOrient;
    shape = new MeshShape(squareMesh);
    shape->setMat(scene->getMat("light mat"));
    shape->transform(transform);
    scene->addShape(shape);

    transform.makeIdentity();
    transform.translate(11.0,60.0,-11.0);
    transform *= lightOrient;
    shape = new MeshShape(squareMesh);
    shape->setMat(scene->getMat("light mat"));
    shape->transform(transform);
    scene->addShape(shape);

    transform.makeIdentity();
    transform.translate(-11.0,60.0,11.0);
    transform *= lightOrient;
    shape = new MeshShape(squareMesh);
    shape->setMat(scene->getMat("light mat"));
    shape->transform(transform);
    scene->addShape(shape);

    transform.makeIdentity();
    transform.translate(11.0,60.0,11.0);
    transform *= lightOrient;
    shape = new MeshShape(squareMesh);
    shape->setMat(scene->getMat("light mat"));
    shape->transform(transform);
    scene->addShape(shape);

    // Diamond
    transform.makeIdentity();
    transform.translate(-20.0,10.0,0.0);
    transform.scale(5.0,10.0,5.0);
    transform.rotate(Vector(0.0,1.0,0.0), 180.0);
    shape = new MeshShape(diamondMesh);
    shape->setMat(scene->getMat("earth"));
    shape->transform(transform);
    scene->addShape(shape);

    // earth
    transform.makeIdentity();
    transform.translate(-20.0, 5.0, 10.0);
    transform.scale(4.0);
    transform.rotate(Vector(0.0,1.0,0.0), 180.0);
    shape = new Sphere(Vector(0.0,0.0,0.0), 1.0);
    shape->setMat(scene->getMat("earth"));
    shape->transform(transform);
    scene->addShape(shape);


    // spheres
    transform.makeIdentity();
    transform.translate(-8.0, 4.0, -15.0);
    transform.scale(3.9);
    shape = new Sphere(Vector(0.0,0.0,0.0), 1.0);
    shape->setMat(scene->getMat("flat red"));
    shape->transform(transform);
    scene->addShape(shape);

    transform.makeIdentity();
    transform.translate(6.0,4.0,-15.0);
    transform.scale(8.0,3.9,4.0);
    shape = new Sphere(Vector(0.0,0.0,0.0), 1.0);
    shape->setMat(scene->getMat("flat blue"));
    shape->transform(transform);
    scene->addShape(shape);

    transform.makeIdentity();
    transform.translate(20.0, 4.0, -15.0);
    transform.scale(3.9);
    shape = new Sphere(Vector(0.0,0.0,0.0), 1.0);
    shape->setMat(scene->getMat("flat red"));
    shape->transform(transform);
    scene->addShape(shape);

    transform.makeIdentity();
    transform.translate(-8.0, 4.0, 0.0);
    transform.scale(3.9);
    shape = new Sphere(Vector(0.0,0.0,0.0), 1.0);
    shape->setMat(scene->getMat("shiny red"));
    shape->transform(transform);
    scene->addShape(shape);

    transform.makeIdentity();
    transform.translate(6.0,4.0,0.0);
    transform.scale(8.0,3.9,4.0);
    shape = new Sphere(Vector(0.0,0.0,0.0), 1.0);
    shape->setMat(scene->getMat("shiny blue"));
    shape->transform(transform);
    scene->addShape(shape);

    transform.makeIdentity();
    transform.translate(20.0, 4.0, 0.0);
    transform.scale(3.9);
    shape = new Sphere(Vector(0.0,0.0,0.0), 1.0);
    shape->setMat(scene->getMat("shiny red"));
    shape->transform(transform);
    scene->addShape(shape);

    transform.makeIdentity();
    transform.translate(-8.0, 4.0, 15.0);
    transform.scale(3.9);
    shape = new Sphere(Vector(0.0,0.0,0.0), 1.0);
    shape->setMat(scene->getMat("dielectric red"));
    shape->transform(transform);
    scene->addShape(shape);

    transform.makeIdentity();
    transform.translate(6.0,4.0,15.0);
    transform.scale(8.0,3.9,4.0);
    shape = new Sphere(Vector(0.0,0.0,0.0), 1.0);
    shape->setMat(scene->getMat("dielectric blue"));
    shape->transform(transform);
    scene->addShape(shape);

    transform.makeIdentity();
    transform.translate(20.0, 4.0, 15.0);
    transform.scale(3.9);
    shape = new Sphere(Vector(0.0,0.0,0.0), 1.0);
    shape->setMat(scene->getMat("dielectric red"));
    shape->transform(transform);
    scene->addShape(shape);


    // Metaballs
    transform.makeIdentity();
    transform.translate(14.0,5.0,27.0);
    transform.rotate(Vector(0.0,1.0,0.0), 35.0);
    transform.scale(2.0);
    Metaballs *balls = new Metaballs(0.5);
    balls->addSphere(Vector(0.0,0.0,0.0), 3.0);
    balls->addSphere(Vector(0.0,2.0,0.0), 2.0);
    balls->addSphere(Vector(0.0,3.5,0.0), 1.5);
    balls->addSphere(Vector(-1.0,4.0,0.5), 1.0);
    balls->addSphere(Vector(1.0,4.0,0.5), 1.0);
    balls->setMat(scene->getMat("metaball mat"));
    balls->transform(transform);
    scene->addShape(balls);


    return scene;
}

static Scene *DOFtest(){
    Scene *scene = new Scene();


    Vector e(0.0,6.0,4.0); // Camera position
    Vector lookAt(0.0,2.0,-16.0); // Point of focus
    scene->setCam(new Camera(e,
                             (lookAt-e).normalized(),
                             Vector(0.0,1.0,0.0),
                             (lookAt-e).length(),
                             40.0, 22.5,
                             0.5));


    scene->addLight(new PointLight( e+Vector(16.0,16.0,0.0),
                                    Colour(0.7,0.7,0.7)));


    Material *mat = new Material();
    mat->makePhongAmb(	Colour(0.05,0.05,0.05),
                        Colour(0.8,0.8,0.8),
                        Colour(0.2,0.2,0.2),
                        128.0);
    scene->setMat(string("dull white"), mat);

    mat = new Material();
    mat->makePhongAmb(	Colour(0.05,0.05,0.05),
                        Colour(0.1,0.1,0.1),
                        Colour(0.8,0.8,0.8),
                        128.0);
    scene->setMat(string("shiny white"), mat);



    scene->setSkyBox( readJPG((char*)"..\\textures\\skybox_sun\\box-x.jpg"),
                      readJPG((char*)"..\\textures\\skybox_sun\\box+x.jpg"),
                      readJPG((char*)"..\\textures\\skybox_sun\\box-y.jpg"),
                      readJPG((char*)"..\\textures\\skybox_sun\\box+y.jpg"),
                      readJPG((char*)"..\\textures\\skybox_sun\\box-z.jpg"),
                      readJPG((char*)"..\\textures\\skybox_sun\\box+z.jpg") );



    Mesh *mesh = new Mesh();
    mesh->addVertex(Vector(-16.0f,0.0f,-32.0f));
    mesh->addVertex(Vector(-16.0f,0.0f,0.0f));
    mesh->addVertex(Vector(16.0f,0.0f,0.0f));
    mesh->addVertex(Vector(16.0f,0.0f,-32.0f));
    mesh->addUVCoord(0.0f,1.0f);
    mesh->addUVCoord(0.0f,0.0f);
    mesh->addUVCoord(1.0f,0.0f);
    mesh->addUVCoord(1.0f,1.0f);
    mesh->addFacePoint(0);
    mesh->addFacePoint(1);
    mesh->addFacePoint(2);
    mesh->addFacePoint(0);
    mesh->addFacePoint(2);
    mesh->addFacePoint(3);
    MeshShape *meshShape = new MeshShape(mesh);
    meshShape->setMat(scene->getMat("dull white"));
    scene->addShape(meshShape);



    Sphere *sphr = new Sphere(Vector(8.0,2.0,-24.0), 2.0);
    sphr->setMat(scene->getMat("shiny white"));
    scene->addShape(sphr);

    sphr = new Sphere(Vector(4.0,2.0,-20.0), 2.0);
    sphr->setMat(scene->getMat("dull white"));
    scene->addShape(sphr);

    sphr = new Sphere(Vector(0.0,2.0,-16.0), 2.0);
    sphr->setMat(scene->getMat("shiny white"));
    scene->addShape(sphr);

    sphr = new Sphere(Vector(-4.0,2.0,-12.0), 2.0);
    sphr->setMat(scene->getMat("dull white"));
    scene->addShape(sphr);

    sphr = new Sphere(Vector(-8.0,2.0,-8.0), 2.0);
    sphr->setMat(scene->getMat("shiny white"));
    scene->addShape(sphr);


    return scene;
}

}

#endif
