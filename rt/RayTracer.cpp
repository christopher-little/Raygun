#include "RayTracer.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

#include "ImageIO.h"
#include "Light.h"
#include "AABB.h"
#include "Mesh.h"
#include "Shader.h"



// epsilon - allowable margin of error ( i.e. 0.9999 == 1.0 )
const float epsilon = 0.0001f;
// value of pi
const float pi = 3.141592f;




// Rendering parameters that must be explicitly set
Vector	rtSkyBoxSize;				// Physical dimensions of the cube map sky box. Assumes ray will originate at center of these dimensions.

// Variables used for rendering
Vector	_rtSkyBoxMin, _rtSkyBoxMax;	// Minimum and maximum of the skybox AABB. For now these are in the ray tracer, but the textures themselves are in the Scene.




RayTracer::RayTracer(Scene *s)
{
    // Warm up the number generator
    srand((unsigned)time(0));


    // Set the skybox AABB properties
    rtSkyBoxSize = Vector(100.0, 100.0, 100.0);
    _rtSkyBoxMin = rtSkyBoxSize*-0.5f;
    _rtSkyBoxMax = rtSkyBoxSize*0.5f;


    // Assign the default rendering properties
    setDefaultParameters();

    //Store reference to provided scene object
    scene = s;
}

RayTracer::~RayTracer()
{
    delete scene;
}



void RayTracer::setDefaultParameters()
{
    rtDefaultColour = Colour();
    rtClipNear = epsilon;
    rtClipFar = 5000.0f;
    rtRefrIndex = 1.0f;
    rtDepthMax = 5;
    rtPixelSampleCount = 1;
    rtCastShadows = true;
    rtCastAmbient = false;
    rtAmbientSampleCount = 1;
}



void RayTracer::render(ImageBuffer *buf)
{
    // "Render" some random pixels
    buf->rainbowStatic();

    if(scene->cam() == NULL)
    {
        cout << "Camera object has not been instantiated. Has a scene been loaded?" << endl;
        return;
    }

    // u-v-w camera space coordinates
    Vector w = scene->cam()->g() * -1.0;
    Vector u = scene->cam()->t().cross(w).normalized();
    Vector v = w.cross(u).normalized();

    // Cast ray for each pixel
    for(int row=0; row<buf->height(); row++)
    {
        for(int col=0; col<buf->width(); col++)
        {
            Colour pixelColour, sampleColour;
            for(int pix=0; pix<rtPixelSampleCount; pix++)
            {
                float offU, offV;
                // Compute sample position on 'camera lens'
                Vector lensSamplePoint = scene->cam()->e();
                if(rtPixelSampleCount > 1 && scene->cam()->r() > epsilon){
                    offU = (float)rand()/(float)RAND_MAX;
                    offV = (float)rand()/(float)RAND_MAX;

                    float lens_u_s = -1.0f*scene->cam()->r() + 2.0f*scene->cam()->r()*offU;
                    float lens_v_s = -1.0f*scene->cam()->r() + 2.0f*scene->cam()->r()*offV;

                    lensSamplePoint = scene->cam()->e() + u*lens_u_s + v*lens_v_s;
                }

                // Compute pixel sample point on image plane
                offU=offV=0.5;
                if(rtPixelSampleCount > 1)
                {
                    offU = (float)rand()/(float)RAND_MAX;
                    offV = (float)rand()/(float)RAND_MAX;
                }
                float u_s = -0.5f*scene->cam()->w() + scene->cam()->w()*(((float)col+offU)/(float)buf->width());
                float v_s = -0.5f*scene->cam()->h() + scene->cam()->h()*(((float)row+offV)/(float)buf->height());

                // Convert u-v sample coords to x-y-z position (world space)
                Vector s = scene->cam()->e() + u*u_s + v*v_s + w*-scene->cam()->N();

                // Create the ray
                Ray ray(lensSamplePoint, (s-lensSamplePoint).normalized());

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
    float nearest_t=-1.0f;
    Vector nearest_p;
    Vector nearest_n;
    float nearest_u=0.0f, nearest_v=0.0f;
    float nearest_t_temp=-1.0f;
    Vector nearest_p_temp;
    Vector nearest_n_temp;
    float nearest_u_temp=0.0f, nearest_v_temp=0.0f;

    int nShapes = scene->nShapes();
    for(int i=0; i<nShapes; i++)
    {
        if(scene->getShape(i)->intersect(ray, nearest_t_temp, nearest_p_temp, nearest_n_temp, nearest_u_temp, nearest_v_temp))
        {
            // Store a new nearest intersection
            if(nearest_t_temp > 0.0f && (nearestShape < 0 || nearest_t_temp < nearest_t))
            {
                nearestShape = i;
                nearest_t = nearest_t_temp;
                nearest_p = nearest_p_temp;
                nearest_n = nearest_n_temp;
                nearest_u = nearest_u_temp;
                nearest_v = nearest_v_temp;
            }
        }
    }


    // When there is no intersection, sample an environment map or return a default colour
    if(nearestShape < 0)
    {
        // Sample the skybox if it exists, otherwise use the default ray tracer colour
        if(scene->getSkyBox(0) != NULL)
        {
            int enter_face=-1, exit_face=-1;
            float enter_t=-1.0f, exit_t=-1.0f;
            // Assume ray starting from 0,0,0 will always intersect the skybox, unless it's malformed and doesn't surround 0,0,0; in which case it needs to be fixed.
            if(!rayBoxIntersect(Vector(0.0,0.0,0.0), ray.d(), _rtSkyBoxMin, _rtSkyBoxMax, enter_face, exit_face, enter_t, exit_t))
                cout << "Sky box is not properly placed around the origin (0,0,0)" << endl;

            // Calculate skybox AABB intersection position
            Vector pSkyBox = (Vector(0.0,0.0,0.0) + ray.d()*(exit_t-epsilon) - _rtSkyBoxMin);
            float u=0.0f, v=0.0f;
            if(exit_face == 0 || exit_face == 1)
            {
                u = pSkyBox.z() / rtSkyBoxSize.z();
                if(exit_face == 0)
                    u = 1.0f-u;
                v = pSkyBox.y() / rtSkyBoxSize.y();
            }
            else if(exit_face == 2 || exit_face == 3)
            {
                u = pSkyBox.x() / rtSkyBoxSize.x();
                v = pSkyBox.z() / rtSkyBoxSize.z();
                if(exit_face == 2)
                    v=1.0f-v;
            }
            else if(exit_face == 4 || exit_face == 5)
            {
                u = pSkyBox.x() / rtSkyBoxSize.x();
                if(exit_face == 5)
                    u = 1.0f-u;
                v = pSkyBox.y() / rtSkyBoxSize.y();
            }
            else
                cout << "ANOTHER PROBLEM" << endl;

            if(u >= 1.0f)
                u = 1.0f-epsilon;
            if(v >= 1.0f)
                v = 1.0f-epsilon;

        // Compute texture position, texture texel position, and fractional distance inside texel
            ImageBuffer *skyTex = scene->getSkyBox(exit_face);
            float uT = u*(skyTex->width()-1);
            float vT = v*(skyTex->height()-1);
            int uI = static_cast<int>(uT);
            int vI = static_cast<int>(vT);
            float uD = uT-uI;
            float vD = vT-vI;

            // Bilinear interpolation across the 4 nearest texels
            Colour c0 = skyTex->getPixel(vI, uI);
            Colour c1 = skyTex->getPixel(vI, uI+1);
            Colour c2 = skyTex->getPixel(vI+1, uI);
            Colour c3 = skyTex->getPixel(vI+1, uI+1);

            Colour d0 = c0 + (c1*uD + c0*-uD);
            Colour d1 = c2 + (c3*uD + c2*-uD);

            Colour d3 = d0 + (d1*vD + d0*-vD);
            return d3;
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
            if(nearest_n.dot(l) < epsilon)
                continue;

            // Check for shadows (light direction ray occluded)
            shadow = false;
            if(rtCastShadows)
            {
                if(sampleTrace(Ray(nearest_p, l), epsilon, 1.0))
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

        // Average the light value received from each light sample
        diffuseIllum = diffuseIllum + sampleDiffuseIllum/static_cast<float>(lightPoints.size());
        specularIllum = specularIllum + sampleSpecularIllum/static_cast<float>(lightPoints.size());
    }
    // Clamp the resulting illumination values
    ambient = ambient.clamp();
    diffuseIllum = diffuseIllum.clamp();
    specularIllum = specularIllum.clamp();



    // Fresnel reflection, R is proportion reflected, 1-R is proportion refracted (when material is dielectric)
    // ***This needs to be replaced with proper modelling
    float fresnelR = 1.0f;

    // Specular reflection
    if(depth < rtDepthMax && mat->s().visible())
    {
        // Compute reflection ray direction
        Vector r = ray.d() - nearest_n*(2*ray.d().dot(nearest_n));
        // Move surface point "up" a bit by progressing slighly along normal vector
        Ray reflRay(nearest_p + nearest_n*epsilon, r);
        //Trace reflected ray (note trace() clamps the returned colour value)
        specularRefl = trace(reflRay, clipNear, clipFar-nearest_t, depth+1);
    }

    // Specular refraction / transmission
    if(mat->isDielectric())
    {
        // Fresnel factor
        // ***This needs to be replaced with proper modelling
        fresnelR = 0.2f;


        // Refract the ray entering the shape
        float refrRatio = rtRefrIndex/mat->n();
        float cosThetaInc = nearest_n.dot(ray.d()*-1.0f);
        float cosThetaTrans = sqrt(1.0f - (refrRatio*refrRatio)*(1.0f - cosThetaInc*cosThetaInc));
        Vector refrDir;
        if(cosThetaInc >= epsilon)
            refrDir = ray.d()*refrRatio + nearest_n*(refrRatio*cosThetaInc - cosThetaTrans);
        else
            refrDir = ray.d()*refrRatio + nearest_n*(-refrRatio*cosThetaInc + cosThetaTrans);
        refrDir = refrDir.normalized();
        Ray refrRay(nearest_p + nearest_n*-epsilon, refrDir);

        // Find where refracted ray exits
        float exit_t;
        Vector exit_p;
        Vector exit_n;
        float temp_u, temp_v;
        scene->getShape(nearestShape)->intersect(refrRay, exit_t, exit_p, exit_n, temp_u, temp_v);

        // Refract the ray again and trace further
        refrRatio = mat->n()/rtRefrIndex;
        cosThetaInc = exit_n.dot(refrRay.d());
        cosThetaTrans = sqrt(1.0f - (refrRatio*refrRatio)*(1.0f - cosThetaInc*cosThetaInc));
        if(cosThetaInc >= epsilon)
            refrDir = refrRay.d()*refrRatio + exit_n*(refrRatio*cosThetaInc - cosThetaTrans);
        else
            refrDir = refrRay.d()*refrRatio + exit_n*(-refrRatio*cosThetaInc + cosThetaTrans);
        refrDir = refrDir.normalized();
        refrRay = Ray(exit_p + exit_n*epsilon, refrDir);

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
        }

        ambientOcclusion -= ambientSample/rtAmbientSampleCount;
    }


    // Compute colour value
    return Shader::phong(mat, ambient, diffuseIllum, specularIllum, specularRefl, specularRefr, fresnelR, ambientOcclusion, nearest_p, nearest_n, nearest_u, nearest_v);
}


bool RayTracer::sampleTrace(const Ray &ray, float clipNear, float clipFar)
{
    float nearest_t_temp;
    Vector nearest_p_temp;
    Vector nearest_n_temp;
    float nearest_u_temp, nearest_v_temp;

    for(int i=0; i<scene->nShapes(); i++)
    {
        // Ray origin is offset by epsilon from the surface to avoid rounding error
        if(scene->getShape(i)->intersect(ray, nearest_t_temp, nearest_p_temp, nearest_n_temp, nearest_u_temp, nearest_v_temp))
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
