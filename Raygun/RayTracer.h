#pragma once

#ifndef RAY_TRACER
#define RAY_TRACER

////////////////////////////////////////////////////////////////////////////////
// class RayTracer
//
// This object manages all aspects of ray tracing
////////////////////////////////////////////////////////////////////////////////
#include "ImageBuffer.h"
#include "Camera.h"
#include "Scene.h"
#include "Ray.h"

class RayTracer
{
public:
	// RayTracer() is going to be the main set up function to make sure ray tracer operations
	// are separate from front end (win32) code
	RayTracer();
	virtual ~RayTracer();


	// Render the currently defined scene and store in the provided buffer
	void render(ImageBuffer *buf);

private:
	Scene *scene;	// Scene to be ray traced

	/**
	 * Trace the given ray through the scene, compute shading and return a colour value.
	 * 
	 * @param ray Ray object being cast into the scene.
	 * @param clipNear Near clipping distance. Minimum distance from camera for a valid intersection.
	 * @param clipFar Far clipping distanct. Maximum distance from camera for a valid intersection.
	 * @param depth Maximum number of ray bounces (reflections, etc.).
	 * 
	 * @param [out] returnColour Resulting Colour object computed for the Ray.
	 *
	 * @return Returns TRUE if an intersection is found and a colour value is computed.
	 */
	Colour trace(const Ray &ray, float clipNear, float clipFar, int depth);

	/**
	 * Similar to trace() function, however only determines 'if' an intersection occurs. Does not compute colour
	 * or which object is intersected.
	 *
	 * @param ray Ray object cast into the scene.
	 * @param clipNear Near clipping distance. Minimum distance from camera for a valid intersection.
	 * @param clipFar Far clipping distanct. Maximum distance from camera for a valid intersection.
	 *
	 * @return Returns TRUE if any intersection occurs with the Ray.
	 */
	bool sampleTrace(const Ray &ray, float clipNear, float clipFar);
};

#endif