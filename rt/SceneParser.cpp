#include <SceneParser.h>

#include <cstring>
#include <cassert>

using namespace std;

Scene* SceneParser::parse_yaml(const char *filename){
    bool cameraFound = false;
    bool atLeastOneShapeFound = false;

    Scene *outputScene = new Scene();

    // Load the yaml tree from the given file
    YAML::Node node = YAML::LoadFile(filename);

    // Iterate through list of entities
    for(YAML::const_iterator it=node.begin(); it != node.end(); it++){
        // Retrieve entity object type name
        YAML::const_iterator n = it->begin();
        string objectType = n->first.as<string>();


        // Camera entity found
        if(objectType == "camera"){
            if(!cameraFound){
                cameraFound = true;
                outputScene->setCam(yaml_camera(n->second));
            }
            else
                cout << "Warning: multiple camera entities defined in yaml scene file. Only first camera declaration will be used." << endl;
        }


        // Shape entity found
        else if(objectType == "shape"){
            atLeastOneShapeFound = true;
            Shape *shape = NULL;

            // Read the shape type name and parse accordingly
            YAML::Node shapeTypeNode = n->second["type"];
            if(shapeTypeNode.IsDefined()){
                string shapeTypeString = shapeTypeNode.as<string>();

                // Parse a sphere
                if(shapeTypeString == "sphere"){
                    shape = yaml_sphere(n->second);
                }

                // Shape type is not supported
                else
                    cout << "Could not parse shape of type: " << shapeTypeString << endl;
            }
            else
                cout << "Shape 'type' could not be determined. Is it formatted correctly?" << endl;


            // Add the parsed shape to the scene
            if(shape != NULL)
                outputScene->addShape(shape);
        }


        // Light entity found
        else if(objectType == "light"){
            Light *light = NULL;

            // Read the light type name and parse accordingly
            YAML::Node lightTypeNode = n->second["type"];
            if(lightTypeNode.IsDefined()){
                string lightTypeString = lightTypeNode.as<string>();

                // Parse a point light
                if(lightTypeString == "point"){
                    light = yaml_pointlight(n->second);
                }
                else
                    cout << "Could not parse light of type: " << lightTypeString << endl;
            }
            else
                cout << "Light 'type' could not be determined. Is it formatted correctly?" << endl;


            // Add the parsed light to the Scene
            if(light != NULL)
                outputScene->addLight(light);
        }


        // Material entity found
        else if(objectType == "material"){
            {}
        }


        // Texture entity found
        else if(objectType == "texture"){
            {}
        }
    }


    if(!cameraFound){
        {} //*** NEED A DEFAULT CAMERA
    }


    return outputScene;
}







Vector SceneParser::yaml_vector(YAML::Node){
    Vector v;
    return v;
}



Camera* SceneParser::yaml_camera(YAML::Node){
    Camera *cam = NULL;
    return cam;
}


PointLight* SceneParser::yaml_pointlight(YAML::Node){
    PointLight *pl = NULL;
    return pl;
}


Sphere* SceneParser::yaml_sphere(YAML::Node){
    Sphere *sphr = NULL;
    return sphr;
}


Material* SceneParser::yaml_material(YAML::Node){
    Material *mat = NULL;
    return mat;
}


ImageBuffer* SceneParser::yaml_texture(YAML::Node){
    ImageBuffer *tex = NULL;
    return tex;
}
