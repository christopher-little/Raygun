#include "ImageIO.h"

ImageBuffer *readJPG(char *filename)
{
	ImageBuffer *returnBuffer;

	struct jpeg_decompress_struct imgInfo;
	struct jpeg_error_mgr jerr;
	imgInfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&imgInfo);

	FILE *imgfile;
	if((imgfile = fopen(filename, "rb")) == NULL)
	{
		return NULL;
	}
	jpeg_stdio_src(&imgInfo, imgfile);
	jpeg_read_header(&imgInfo, true);
	jpeg_start_decompress(&imgInfo);

	// Allocate buffer to store loaded jpeg
	returnBuffer = new ImageBuffer(imgInfo.output_width, imgInfo.output_height);

	// Allocate rgb array and read in jpeg file line by line
	float *raw_image = new float[imgInfo.output_width *
									imgInfo.output_height *
									imgInfo.num_components];
	JSAMPLE *raw_row = new JSAMPLE[imgInfo.output_width *
									imgInfo.num_components];

	int location = 0;
	while(imgInfo.output_scanline < imgInfo.image_height)
	{
		jpeg_read_scanlines(&imgInfo, &raw_row, 1);
		for(int i=0; i<imgInfo.output_width*imgInfo.num_components; i++)
			raw_image[location++] = raw_row[i]/255.0f;
	}

	// Clean up
	jpeg_finish_decompress(&imgInfo);
	jpeg_destroy_decompress(&imgInfo);
	delete [] raw_row;
	fclose(imgfile);


	// Image comes in as BGR for some reason, so flip it round to RGB
	for(int i=0; i<returnBuffer->width()*returnBuffer->height(); i++)
	{
		float temp = raw_image[i*3];
		raw_image[i*3] = raw_image[i*3 + 2];
		raw_image[i*3 + 2] = temp;
	}

	returnBuffer->setBuffer(raw_image);

	return returnBuffer;
}