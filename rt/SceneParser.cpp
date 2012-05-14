#include <SceneParser.h>

#include <iostream>
#include <yaml-cpp/yaml.h>

Scene* SceneParser::parse_yaml(const char *filename){
    YAML::Node node = YAML::LoadFile(filename);
    std::cout << node.size() << std::endl;

    Scene *outputScene = NULL;
    return outputScene;
}
