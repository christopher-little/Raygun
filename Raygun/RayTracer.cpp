#include "RayTracer.h"

#include <ctime>

#include "TRACE.h"
#include "Light.h"

#include "AABB.h"

// epsilon - allowable margin of error ( 0.9999 == 1.0 )
const float eps = 0.0001f;
// value of pi
const float pi = 3.141592f;

// Rendering parameters (static)
Colour	rtDefaultColour;			// Default background colour (is set to black by default)
float	rtClipNear = eps;			// Near and far ray clipping distance
float	rtClipFar = 5000.0f;
float	rtRefrIndex = 1.0f;			// Refractive index of "air" (empty space of the scene)
int		rtDepthMax = 2;				// Maximum recursion count, i.e. maximum number of reflections
int		rtPixelSampleCount = 1;		// Number of samples per pixel
bool	rtCastShadows = true;		// Cast shadow rays
bool	rtCastAmbient = false;		// Cast ambient rays
int		rtAmbientSampleCount = 4;
// Rendering parameters that must be explicitly set
Vector	rtSkyBoxSize;				// Physical dimensions of the cube map sky box. Assumes ray will originate at center of these dimensions.

// Variables used for rendering
Vector	_rtSkyBoxMin, _rtSkyBoxMax;	// Minimum and maximum of the skybox AABB. For now these are in the ray tracer, but the textures themselves are in the Scene.



RayTracer::RayTracer()
{
	// Warm up the number generator
	srand((unsigned)time(0));

	
	// Pull in an MDL scene file
	scene = new Scene();
	//cam = scene->loadMDL("..\\test_scenes\\cornellbox.mdla");
	//cam = scene->loadMDL("..\\test_scenes\\cornellboxRGB.mdla");
	//cam = scene->loadMDL("..\\test_scenes\\basic_spheres.mdla");
	//cam = scene->loadMDL("..\\test_scenes\\basic_spheres_angled.mdla");
	cam = scene->loadMDL("..\\test_scenes\\skybox_platform.mdla");


	// Create a skybox
	rtSkyBoxSize = Vector(200.0, 200.0, 200.0);
	_rtSkyBoxMin = rtSkyBoxSize*-0.5f;
	_rtSkyBoxMax = rtSkyBoxSize*0.5f;

	ImageBuffer temp(256, 256);
	temp.rainbowStatic();
	for(int i=0; i<6; i++)
		scene->setSkyBoxTex(i, temp.makeCopy(), temp.width(), temp.height());
}

RayTracer::~RayTracer()
{
    delete scene;
}



void RayTracer::render(ImageBuffer *buf)
{
	// "Render" some random pixels
	buf->rainbowStatic();

	// u-v-w camera space coordinates
	Vector w = cam->g() * -1.0;
	Vector u = cam->t().cross(w).normalized();
	Vector v = w.cross(u).normalized();

	// Cast ray for each pixel
	for(int row=0; row<buf->height(); row++)
	{
		for(int col=0; col<buf->width(); col++)
		{
			Colour pixelColour, sampleColour;
			for(int pix=0; pix<rtPixelSampleCount; pix++)
			{
				// Compute u-v coords of sample position on the image plane
				float offU, offV;
				offU = offV = 0.5; //<--Mess with this [0.0 .. 1.0] to randomize samples (distributed ray tracing)
				if(rtPixelSampleCount > 1)
				{
					offU = (float)rand()/(float)RAND_MAX;
					offV = (float)rand()/(float)RAND_MAX;
				}
				float u_s = -0.5f*cam->w() + cam->w()*(((float)col+offU)/(float)buf->width());
				float v_s = -0.5f*cam->h() + cam->h()*(((float)row+offV)/(float)buf->height());

				// Convert u-v sample coords to x-y-z position (world space)
				Vector s = cam->e() + u*u_s + v*v_s + w*-cam->N();

				// Create the ray
				Ray ray(cam->e(), (s-cam->e()).normalized());

				if(row == 64 && col == 448)
				{
					//***Spot for instrumentation TRACE("THING %s\n", ray);
				}

				// Trace the ray
				pixelColour = pixelColour + trace(ray, rtClipNear, rtClipFar, 0);
			}

			// Average the distributed samples
			if(rtPixelSampleCount > 1)
			{
				pixelColour = (pixelColour / (float)rtPixelSampleCount).clamp();
			}

			buf->setPixel(row, col, pixelColour);
		}
	}
}



Colour RayTracer::trace(const Ray &ray, float clipNear, float clipFar, int depth)
{
	// Check each shape for an intersection
	int nearestShape = -1;
	float nearest_t = -1.0;
	Vector nearest_p;
	Vector nearest_n;
	float nearest_t_temp;
	Vector nearest_p_temp;
	Vector nearest_n_temp;
	
	int nShapes = scene->nShapes();
	for(int i=0; i<nShapes; i++)
	{
		if(scene->getShape(i)->intersect(ray, nearest_t_temp, nearest_p_temp, nearest_n_temp))
		{
			// Store a new nearest intersection
			if(nearest_t_temp > 0.0f && (nearestShape < 0 || nearest_t_temp < nearest_t))
			{
				nearestShape = i;
				nearest_t = nearest_t_temp;
				nearest_p = nearest_p_temp;
				nearest_n = nearest_n_temp;
			}
		}
	}
	

	// When there is no intersection, sample an environment map or return a default colour
	if(nearestShape < 0)
	{
		// Sample the skybox if it exists, otherwise use the default ray tracer colour
		if(scene->skyBox())
		{
			int enter_face, exit_face;
			float enter_t, exit_t;
			// Assume ray starting from 0,0,0 will always intersect the skybox, unless it's malformed and doesn't surround 0,0,0; in which case it needs to be fixed.
			if(!rayBoxIntersect(Vector(0.0,0.0,0.0), ray.d(), _rtSkyBoxMin, _rtSkyBoxMax, enter_face, exit_face, enter_t, exit_t))
				TRACE("Sky box is not properly placed around the origin (0,0,0)\n");

			// Calculate skybox AABB intersection position
			Vector pSkyBox = (Vector(0.0,0.0,0.0) + ray.d()*exit_t) - _rtSkyBoxMin;
			float u, v;
			if(exit_face == 0 || exit_face == 1)
			{
				u = pSkyBox.z() / rtSkyBoxSize.z();
				v = pSkyBox.y() / rtSkyBoxSize.y();
			}
			else if(exit_face == 2 || exit_face == 3)
			{
				u = pSkyBox.x() / rtSkyBoxSize.x();
				v = pSkyBox.z() / rtSkyBoxSize.z();
			}
			else if(exit_face == 4 || exit_face == 5)
			{
				u = pSkyBox.x() / rtSkyBoxSize.x();
				v = pSkyBox.y() / rtSkyBoxSize.y();
			}
			else
				TRACE("ANOTHER PROBLEM\n");

			return scene->getSkyBoxTex(exit_face, u, v);
		}
		else
		{
			if(depth > 0)
				return Colour(0.0, 0.0, 0.0);
			else
				return rtDefaultColour;
		}
	}


	// Retrieve shape material
	Material *mat = scene->getShape(nearestShape)->mat();

	// Direct illumination (diffuse and specular phong shading)
	Colour ambient, diffuseIllum, specularIllum; // Direct illumination (Ambient, diffuse and specular)
	Colour specularRefl, specularRefr; // Reflection (diffuse, specular)
	bool shadow;

	// Sample the light sources
	Colour sampleDiffuseIllum, sampleSpecularIllum;
	for(int i=0; i<scene->nLights(); i++)
	{
		Light *light = scene->getLight(i);

		// Grab the ambient component
		ambient = ambient + light->c();

		// Reset the sample colour
		sampleDiffuseIllum = Colour();
		sampleSpecularIllum = Colour();

		// Get a list of light sample points
		vector<Vector> lightPoints = light->getSamplePoints(1);
		for(vector<Vector>::iterator li=lightPoints.begin(); li!=lightPoints.end(); li++)
		{
			// Get the light direction
			Vector l = *li - nearest_p;
			// Bail out if the surface normal points away from light
			if(nearest_n.dot(l) < eps)
				continue;

			// Check for shadows (light direction ray occluded)
			shadow = false;
			if(rtCastShadows)
			{
				if(sampleTrace(Ray(nearest_p, l), eps, 1.0))
					shadow = true;
			}

			// Perform diffuse and specular shading when not in shadow
			if(!shadow)
			{
				l = l.normalized();
				// Only compute illumination if diffuse or specular components are valid/non-zero)
				if(mat->d().visible())
				{
					sampleDiffuseIllum = sampleDiffuseIllum + light->c()*nearest_n.dot(l);
				}
				if(mat->s().visible())
				{
					Vector h = (ray.d()*-1.0 + l).normalized();
					sampleSpecularIllum  = sampleSpecularIllum + light->c() * pow(h.dot(nearest_n), mat->p());
				}
			}
		}

		diffuseIllum = diffuseIllum + sampleDiffuseIllum/lightPoints.size();
		specularIllum = specularIllum + sampleSpecularIllum/lightPoints.size();
	}
	// Clamp the resulting illumination values
	ambient = ambient.clamp();
	diffuseIllum = diffuseIllum.clamp();
	specularIllum = specularIllum.clamp();



	// Fresnel reflection, R is proportion reflected, 1-R is proportion refracted (when material is dielectric)
	float fresnelR = 1.0f;

	// Specular reflection
	if(depth < rtDepthMax && mat->s().visible())
	{
		// Compute reflection ray direction
		Vector r = ray.d() - nearest_n*(2*ray.d().dot(nearest_n));
		// Move surface point "up" a bit by progressing slighly along normal vector
		Ray reflRay(nearest_p + nearest_n*eps, r);
		//Trace reflected ray (note trace() clamps the returned colour value)
		specularRefl = trace(reflRay, clipNear, clipFar-nearest_t, depth+1);
	}

	// Specular refraction / transmission
	if(mat->isDielectric())
	{
		// Refract the ray entering the shape
		float refrRatio = rtRefrIndex/mat->n();
		float cosThetaInc = nearest_n.dot(ray.d()*-1);
		float cosThetaTrans = sqrt(1 - (refrRatio*refrRatio)*(1 - cosThetaInc*cosThetaInc));
		Vector refrDir;
		if(cosThetaInc >= eps)
			refrDir = ray.d()*refrRatio + nearest_n*(refrRatio*cosThetaInc - cosThetaTrans);
		else
			refrDir = ray.d()*refrRatio + nearest_n*(-refrRatio*cosThetaInc + cosThetaTrans);
		refrDir = refrDir.normalized();
		Ray refrRay(nearest_p + nearest_n*-eps, refrDir);

		// Schlick approximation to Fresnel equation
		float R0 = (mat->n() - rtRefrIndex)/(mat->n() + rtRefrIndex);
		R0 = R0*R0;
		fresnelR =	R0 + (1-R0)*pow(1.0 - cosThetaInc, 5);

		// Find where refracted ray exits
		float exit_t;
		Vector exit_p;
		Vector exit_n;
		scene->getShape(nearestShape)->intersect(refrRay, exit_t, exit_p, exit_n);

		// Refract the ray again and trace further
		refrRatio = mat->n()/rtRefrIndex;
		cosThetaInc = exit_n.dot(refrRay.d()*-1);
		//float temp = 1 - (refrRatio*refrRatio)*(1 - cosThetaInc*cosThetaInc);
		cosThetaTrans = sqrt(1 - (refrRatio*refrRatio)*(1 - cosThetaInc*cosThetaInc));
		if(cosThetaInc >= 0.0f)
			refrDir = refrRay.d()*refrRatio + exit_n*(refrRatio*cosThetaInc - cosThetaTrans);
		else
			refrDir = refrRay.d()*refrRatio + exit_n*(-refrRatio*cosThetaInc + cosThetaTrans);
		refrDir = refrDir.normalized();
		refrRay = Ray(exit_p + exit_n*eps, refrDir);

		specularRefr = trace(refrRay, clipNear, clipFar-nearest_t, depth+1);
	}


	// Ambient occlusion
	float ambientOcclusion = 1.0f, ambientSample = 0.0f;
	if(rtCastAmbient)
	{
		for(int amb=0; amb<rtAmbientSampleCount; amb++)
		{
			//generate random ray direction in hemisphere
			Vector ambDir = Vector(((float)rand()/(float)RAND_MAX)*2.0f - 1.0f, ((float)rand()/(float)RAND_MAX)*2.0f - 1.0f, ((float)rand()/(float)RAND_MAX)*2.0f - 1.0f).normalized();
			if(ambDir.dot(nearest_n)  > 0.1f)
				if(sampleTrace(Ray(nearest_p, ambDir), 0.01f, 5.0f))
					ambientSample += 1.0f;
		
			/*
			float u1 = (float)rand()/(float)RAND_MAX;
			float u2 = (float)rand()/(float)RAND_MAX;
			float r = sqrt(1.0f - u1*u1);
			float phi = 2*pi * u2;
			Vector ambDir(cos(phi) * r, sin(phi) * r, u1);
			// transform tangent space random hemisphere ray to world space ray
			Vector w = nearest_n;
			Vector u = cam->t().cross(w);
			Vector v = w.cross(u);
			ambDir = (u*ambDir.x() + v*ambDir.y() + w*ambDir.z()).normalized();
			// sample ambient occlusion ray for intersections
			if(sampleTrace(Ray(nearest_p, ambDir), eps, 5.0))
			{
				ambientOcclusion = 0.0f;
			}
			*/
			/*
			float denom = sqrt(nearest_p.x()*nearest_p.x() + nearest_p.y()*nearest_p.y() + nearest_p.z()*nearest_p.z());
			float maxHalf = (float)RAND_MAX * 0.5f;
			Vector ambientDirection = Vector((float)rand()/maxHalf - 1.0f, (float)rand()/maxHalf - 1.0f, (float)rand()/maxHalf - 1.0f).normalized();
			if(nearest_n.dot(ambientDirection) < 0.0f)
				ambientDirection = ambientDirection*-1.0;
			if(sampleTrace(Ray(nearest_p, ambientDirection), eps, 5.0))
			{
				ambientOcclusion = 0.0f;
			}
			*/
		}

		ambientOcclusion -= ambientSample/rtAmbientSampleCount;
	}


	// Finalize the colour value and return
	Colour returnColour =	ambient*mat->a() +
							diffuseIllum*mat->d() +
							specularIllum*mat->s() +
							specularRefl*mat->s()*fresnelR +
							specularRefr*mat->s()*(1-fresnelR);
	return returnColour.clamp() * ambientOcclusion;
}


bool RayTracer::sampleTrace(const Ray &ray, float clipNear, float clipFar)
{
	float nearest_t_temp;
	Vector nearest_p_temp;
	Vector nearest_n_temp;

	for(int i=0; i<scene->nShapes(); i++)
	{
		// Ray origin is offset by epsilon from the surface to avoid rounding error
		if(scene->getShape(i)->intersect(ray, nearest_t_temp, nearest_p_temp, nearest_n_temp))
		{
			// Ensure that the intersected object is between the surface and the light sample point
			if(nearest_t_temp > clipNear && nearest_t_temp < clipFar)
			{
				return true;
			}
		}
	}

	return false;
}