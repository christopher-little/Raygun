#include "ImageBuffer.h"

#include "ImageIO.h"
#include "Colour.h"


void ImageBuffer::generatePerlin(Perlin &p)
{
	for(int row=0; row<_height; row++) for(int col=0; col<_width; col++)
	{
		// Grab a noise value for the pixel and shift range from [-1..1] to [0..1]
		float noise = p.noise((float)col/_width, (float)row/_height, 0.0f);
		noise = (noise+1.0f)*0.5f;
			
		// Remember:						 x,									 y      (not row,col)
		setPixel(row, col, Colour(noise,noise,noise));
	}
}


void ImageBuffer::warpPerlin(Perlin &p)
{
	// Make a copy of the current image buffer. This is used to preserve the original texture while the current texture is warped.
	ImageBuffer *newBuf = new ImageBuffer(_width, _height);
	memcpy(newBuf->getBuffer(), buffer, _width*_height*3*sizeof(float));

	for(int row=0; row<_height; row++) for(int col=0; col<_width; col++)
	{
		// Make some noise at current pixel
		float x = (float)col/_width;
		float y = (float)row/_height;

		float noise = p.noise(x,y,0.0f)*0.13f;

		// Find noisey pixel position. If noise pushes pixel position beyond image dimension, wrap around.
		int noiseCol = static_cast<int>(col + _width*noise);
		if(noiseCol > _width-1) noiseCol -= _width;
		else if(noiseCol < 0) noiseCol += _width;
		int noiseRow = static_cast<int>(row + _height*noise);
		if(noiseRow > _height-1) noiseRow -= _height;
		else if(noiseRow < 0) noiseRow += _height;

		// Set noisy pixel position from copied buffer to current buffer
		setPixel( row, col, newBuf->getPixel(_height-1-noiseRow, noiseCol) );
	}

	delete newBuf;
}



void ImageBuffer::rainbowStatic()
{
	for(int row=0; row<_height; row++) for(int col=0; col<_width; col++)
	{
		Colour out(	(float)rand()/(float)RAND_MAX,
					(float)rand()/(float)RAND_MAX,
					(float)rand()/(float)RAND_MAX);
		setPixel(row, col, out);
	}
}



void ImageBuffer::gradient()
{
	for(int row=0; row<_height; row++) for(int col=0; col<_width; col++)
	{
		Colour out(	((float)row/(float)_height)*1.0f,
					0.0f,
					((float)col/(float)_width)*1.0f);
		setPixel(row, col, out);
	}
}