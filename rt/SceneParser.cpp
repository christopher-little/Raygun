#include <SceneParser.h>

#include <cstring>
#include <cassert>

#include <ImageIO.h>

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



            if(shape != NULL){
                // Ensure the shape references a valid material
                YAML::Node shapeMatRef = n->second["material"];
                if(shapeMatRef.IsDefined()){
                    // Ensure the referenced shape material exists
                    Material *shapeMat = outputScene->getMat(shapeMatRef.as<string>());
                    if(shapeMat != NULL){
                        shape->setMat(shapeMat);
                        outputScene->addShape(shape);
                    }
                    else{
                        delete shape;
                        cout << "Shape material reference does not exist." << endl;
                    }
                }
                else{
                    delete shape;
                    cout << "Shape is missing reference to material." << endl;
                }
            }
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
            string matName;
            Material *mat = NULL;

            YAML::Node matNameNode = n->second["name"];
            if(matNameNode.IsDefined()){
                matName = matNameNode.as<string>();

                // Retrieve the basic material properties
                mat = yaml_material(n->second);

                // Map material texture name reference to stored texture
                YAML::Node matTexName = n->second["texture"];
                if(matTexName.IsDefined()){
                    //mat->setTexture(outputScene->getTex(matTexName.as<string>()));
                    ImageBuffer *matTex = outputScene->getTex(matTexName.as<string>());
                    if(matTex != NULL){
                        mat->setTexture(matTex);
                    }
                }
            }
            else
                cout << "Material 'name' property could not be retrieved.";

            if(mat != NULL)
                outputScene->setMat(matName, mat);
        }


        // Texture entity found
        else if(objectType == "texture"){
            string texName;

            // Retrieve the texture name
            YAML::Node texNameNode = n->second["name"];
            if(texNameNode.IsDefined()){
                texName = texNameNode.as<string>();

                // Retrieve the texture file path
                YAML::Node texFilePathNode = n->second["source"];
                if(texFilePathNode.IsDefined()){
                    outputScene->setTex(texName, readJPG(texFilePathNode.as<string>()));
                }
                else
                    cout << "Could not parse texture path." << endl;
            }
            else
                cout << "Texture 'name' property could not be retrieved.";
        }
    }



    if(!atLeastOneShapeFound)
        cout << "Warning: No shapes have been specified in the scene" << endl;

    if(!cameraFound){
        {} //*** NEED A DEFAULT CAMERA
    }


    return outputScene;
}







Vector SceneParser::yaml_vector(YAML::Node n){
    Vector v;
    if(n.size() != 3){
        cout << "Expected a vector (3 numbers, X,Y,Z). Is the vector formatted correctly?" << endl;
        return v;
    }

    v = Vector(n[0].as<float>(),
               n[1].as<float>(),
               n[2].as<float>());

    return v;
}


Colour SceneParser::yaml_colour(YAML::Node n){
    Colour c;
    if(n.size() != 3){
        cout << "Expected a colour (3 numbers, R,G,B). Is the colour formatted correctly?" << endl;
        return c;
    }

    c = Colour(n[0].as<float>(),
               n[1].as<float>(),
               n[2].as<float>());
    return c;
}


Camera* SceneParser::yaml_camera(YAML::Node n){
    Camera *cam = NULL;
    YAML::Node cameraNode;

    cameraNode = n["position"];
    Vector position;
    if(cameraNode.IsDefined()){
        position = yaml_vector(cameraNode);
    }
    else
        cout << "Camera 'position' property not found, defaulting to origin." << endl;

    cameraNode = n["direction"];
    Vector direction(0.0,0.0,-1.0);
    if(cameraNode.IsDefined()){
        direction = yaml_vector(cameraNode);
    }
    else
        cout << "Camera 'direction' property not found, default to -Z direction." << endl;

    cameraNode = n["up_vector"];
    Vector up_direction(0.0,1.0,0.0);
    if(cameraNode.IsDefined()){
        up_direction = yaml_vector(cameraNode);
    }
    else
        cout << "Camera 'up direction' property not found, default to +Y direction." << endl;

    cameraNode = n["image_plane"];
    float plane_w = 16.0, plane_h = 9.0, depth = 5.0;
    if(cameraNode.IsDefined()){
        plane_w = cameraNode[0].as<float>();
        plane_h = cameraNode[1].as<float>();
        depth = cameraNode[2].as<float>();
    }
    else
        cout << "Camera 'image_plane' property not found, using default 16x9 aspect ratio." << endl;


    cam = new Camera(position, direction, up_direction, depth, plane_w, plane_h);
    return cam;
}


PointLight* SceneParser::yaml_pointlight(YAML::Node n){
    PointLight *pl = NULL;
    YAML::Node plNode;

    Vector position;
    plNode = n["position"];
    if(plNode.IsDefined()){
        position = yaml_vector(plNode);
    }
    else{
        cout << "Light 'position' property not found." << endl;
        return NULL;
    }

    Colour colour;
    plNode = n["colour"];
    if(plNode.IsDefined()){
        colour = yaml_colour(plNode);
    }
    else{
        cout << "Light 'colour' property not found." << endl;
        return NULL;
    }


    pl = new PointLight(position, colour);

    return pl;
}


Sphere* SceneParser::yaml_sphere(YAML::Node n){
    Sphere *sphr = NULL;
    YAML::Node sphrNode;

    Vector position;
    sphrNode = n["position"];
    if(sphrNode.IsDefined()){
        position = yaml_vector(sphrNode);
    }

    float radius = 1.0;
    sphrNode = n["radius"];
    if(sphrNode.IsDefined()){
        radius = sphrNode.as<float>();
    }

    sphr = new Sphere(position, radius);

    return sphr;
}


Material* SceneParser::yaml_material(YAML::Node n){
    string matName;
    Material *mat = NULL;
    YAML::Node matNode;

    matNode = n["name"];
    if(matNode.IsDefined()){
        matName = matNode.as<string>();
    }
    else{
        cout << "Material name not specified. Name is required for shapes to reference." << endl;
        return NULL;
    }

    Colour ambient(0.1,0.1,0.1);
    matNode = n["ambient"];
    if(matNode.IsDefined()){
        ambient = yaml_colour(matNode);
    }

    Colour diffuse(0.7,0.7,0.7);
    matNode = n["diffuse"];
    if(matNode.IsDefined()){
        diffuse = yaml_colour(matNode);
    }

    Colour specular(1.0,1.0,1.0);
    matNode = n["specular"];
    if(matNode.IsDefined()){
        specular = yaml_colour(matNode);
    }

    float shininess = 64.0;
    matNode = n["shininess"];
    if(matNode.IsDefined()){
        shininess = matNode.as<float>();
    }

    bool isDielectric = false;
    matNode = n["dielectric"];
    if(matNode.IsDefined()){
        isDielectric = matNode.as<bool>();
    }

    float refractionIndex = 1.2;
    matNode = n["refraction_index"];
    if(matNode.IsDefined()){
        refractionIndex = matNode.as<float>();
    }

    mat = new Material();
    if(isDielectric){
        mat->makePhongDielectric(specular, shininess, refractionIndex);
    }
    else{
        mat->makePhongAmb(ambient, diffuse, specular, shininess);
    }

    return mat;
}
