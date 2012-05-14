//==============================================================================
// Support functions for parsing a yaml formatted scene file and producing a
// Scene object.
//
// Author: Chris Little
//==============================================================================

#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include <cstring>

#include <yaml-cpp/yaml.h>

#include <Vector.h>
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
    // Retrieve and store a 3D vector
    static Vector yaml_vector(YAML::Node);
    // Retrieve a camera entity
    static Camera* yaml_camera(YAML::Node);
    // Retrieve a point light entity
    static PointLight* yaml_pointlight(YAML::Node);
    // Retrieve a sphere entity
    static Sphere* yaml_sphere(YAML::Node);
    // Retrieve a material entity
    static Material* yaml_material(YAML::Node);
    // Retrieve a texture entity
    static ImageBuffer* yaml_texture(YAML::Node);
};

#endif // SCENEPARSER_H
