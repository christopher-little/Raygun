//==============================================================================
// Support functions for parsing a yaml formatted scene file and producing a
// Scene object.
//
// Author: Chris Little
//==============================================================================

#ifndef SCENEPARSER_H
#define SCENEPARSER_H

#include <cstring>

#include <Scene.h>

namespace SceneParser{

// Parse the yaml file specified by filename and return a Scene object for rendering
Scene* parse_yaml(const char *filename);

}

#endif // SCENEPARSER_H
