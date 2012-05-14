# -------- Raygun --------- #

Currently, Raygun is a basic toy ray tracer project.

### Features: ###
* GUI interface built with QT framework.
	* Funtional parts are render button and display window.
	* Plan to add scene scripting and saving image to user specified file
* Meshes (triangles, and quads "tesselated" into triangles), spheres
* Object transformations (translation, rotation, scaling)
* Point lights
* Direct diffuse, specular lighting
* Specular reflection
* Transmission/refraction (only for spheres at the moment)
* Basic u,v mapping and texture support
* Perlin Noise generation; used to make basic noisey textures or to modify existing textures (i.e. warping)

### Planned features: ###
* Diffuse reflection
* Area lights
* Depth of field
* Motion blur
* BVH (i.e. octrees)
* Additional shapes: cylinders, perhaps cones, metaballs (blobs)

> Note: some test scenes in test_scenes.h use textures that reference image files that are *not included* with the source code due to copyright concerns, etc.
