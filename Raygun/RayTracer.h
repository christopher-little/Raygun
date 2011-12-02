#pragma once

#ifndef RAY_TRACER
#define RAY_TRACER

////////////////////////////////////////////////////////////////////////////////
// class RayTracer
//
// This object manages casting rays and assembling final image
////////////////////////////////////////////////////////////////////////////////
#include "Colour.h"
#include "ImageBuffer.h"
#include "Camera.h"
#include "Scene.h"
#include "Ray.h"

class RayTracer
{
public:
	// Render parameters
	Colour	rtDefaultColour;		// Default background colour (is set to black by default)
	float	rtClipNear;				// Near and far ray clipping distance
	float	rtClipFar;
	float	rtRefrIndex;			// Refractive index of "air" (empty space of the scene)
	int		rtDepthMax;				// Maximum recursion count, i.e. maximum number of reflections
	int		rtPixelSampleCount;		// Number of samples per pixel
	bool	rtCastShadows;			// Cast shadow rays
	bool	rtCastAmbient;			// Cast ambient occlusion rays
	int		rtAmbientSampleCount;	// Number of ambient occlusion samples


	/**
	 * Constructor. Initialize the ray tracer with a given scene
	 */
	RayTracer(Scene *s);
	~RayTracer();


	/**
	 * Set the default rendering parameters
	 */
	void setDefaultParameters();


	/**
	 * Render the currently defined scene and write image to the provided buffer.
	 *
	 * @param[in,out] buf ImageBuffer to contain the resulting ray traced image
	 */
	void render(ImageBuffer *buf);

private:
	Scene *scene;	// Reference to scene to be ray traced

	/**
	 * Trace the given ray through the scene, compute shading and return a colour value.
	 *
	 * @param[in] ray Ray object being cast into the scene.
	 * @param[in] clipNear Near clipping distance. Minimum distance from camera for a valid intersection.
	 * @param[in] clipFar Far clipping distanct. Maximum distance from camera for a valid intersection.
	 * @param[in] depth Maximum number of ray bounces (reflections, etc.).
	 *
	 * @return A Colour object containing the colour found by the ray
	 */
	Colour trace(const Ray &ray, float clipNear, float clipFar, int depth);

	/**
	 * Similar to trace() function, however only determines 'if' an intersection occurs. Does not compute colour
	 * or which object is intersected.
	 *
	 * @param[in] ray Ray object cast into the scene.
	 * @param[in] clipNear Near clipping distance. Minimum distance from camera for a valid intersection.
	 * @param[in] clipFar Far clipping distanct. Maximum distance from camera for a valid intersection.
	 *
	 * @return Returns TRUE if any intersection occurs with the Ray.
	 */
	bool sampleTrace(const Ray &ray, float clipNear, float clipFar);
};

#endif
