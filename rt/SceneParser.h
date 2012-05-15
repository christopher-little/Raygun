//==============================================================================
// Support functions for parsing a yaml formatted scene file and producing a
// Scene object.
//
// Currently only supports: Basic camera, Point lights, spheres, materials and
// textures. ***Still need to implement: Transformations, mesh, metaballs
//
// Author: Chris Little
//==============================================================================

#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include <cstring>

#include <yaml-cpp/yaml.h>

#include <Vector.h>
#include <Colour.h>
#include <Scene.h>
#include <Camera.h>
#include <shapes/all_shapes.h>
#include <lights/all_lights.h>
#include <Material.h>
#include <ImageBuffer.h>

class SceneParser{
public:
    // Parse the yaml file specified by filename and return a Scene object for rendering
    static Scene* parse_yaml(const char *filename);

private:
    //==========================================================================
    // Convenience functions to parse complex entities
    // Note: textures are retrieved within the parse_yaml() function
    //==========================================================================
    // Retrieve and store a 3D vector
    static Vector yaml_vector(YAML::Node n);
    // Retrieve and store a RGB colour
    static Colour yaml_colour(YAML::Node n);
    // Retrieve a camera entity
    static Camera* yaml_camera(YAML::Node n);
    // Retrieve a point light entity
    static PointLight* yaml_pointlight(YAML::Node n);
    // Retrieve a sphere entity
    static Sphere* yaml_sphere(YAML::Node n);
    // Retrieve a material entity, store its name identifier in matName
    static Material* yaml_material(YAML::Node n);
};

#endif // SCENEPARSER_H
