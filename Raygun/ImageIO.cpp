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

	// Package the jpeg image into an ImageBuffer object
	returnBuffer->setBuffer(raw_image);
	return returnBuffer;
}



void writeJPG(char *filename, ImageBuffer *buf)
{
	struct jpeg_compress_struct imgInfo;
	struct jpeg_error_mgr imgErr;
	imgInfo.err = jpeg_std_error(&imgErr);
	jpeg_create_compress(&imgInfo);

	//char filename[64];
	//sprintf(filename, "results/image%d.jpg", runID);
	FILE *imgFile = fopen(filename, "wb");
	if(imgFile == NULL)
	{
		TRACE("Error: Could not open image output file for %s", filename);
		return;
	}
	jpeg_stdio_dest(&imgInfo, imgFile);

	imgInfo.image_width = buf->width();
	imgInfo.image_height = buf->height();
	imgInfo.input_components = 3;
	imgInfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&imgInfo);
	jpeg_set_quality(&imgInfo, 100, TRUE);
	jpeg_start_compress(&imgInfo, TRUE);

	JSAMPLE *raw_row = new JSAMPLE[imgInfo.image_width * 3];
	int location = 0;
	while(imgInfo.next_scanline < imgInfo.image_height)
	{
		int rowIndex = (imgInfo.image_height-1-imgInfo.next_scanline) * imgInfo.image_width * 3;
		float *bufferRow = &buf->getBuffer()[rowIndex];
		for(int i=0; i<imgInfo.image_width*3; i++)
			raw_row[i] = bufferRow[i] * 255;
		jpeg_write_scanlines(&imgInfo, &raw_row, 1);
	}

	jpeg_finish_compress(&imgInfo);
	jpeg_destroy_compress(&imgInfo);
	delete [] raw_row;
	fclose(imgFile);
}