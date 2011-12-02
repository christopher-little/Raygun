#pragma once

#ifndef IMAGEIO
#define IMAGEIO

#include <cstdio>
#include <string>

#include <jpeglib.h>

#include "ImageBuffer.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// ImageIO
//
// Read/Write float rgb array for various image file types
////////////////////////////////////////////////////////////////////////////////

// Read and write jpeg files from/to an ImageBuffer. Note this depends on the jpeg library reading/writing in R,G,B order.
ImageBuffer *readJPG(string filename);
void writeJPG(string filename, ImageBuffer *buf);

#endif
