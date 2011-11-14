#include <iostream>

#include "RayTracer.h"
#include "ImageBuffer.h"
#include "ImageIO.h"

using namespace std;

int main(int argc, char **argv)
{
	int w = 640;
	int h = 360;
	RayTracer rt;
	ImageBuffer *buf = new ImageBuffer(w,h);

	rt.render(buf);
	writeJPG("test.jpg", buf);

	return 0;
}
