//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"

template <typename T>
T clamp(T value, T min, T max) {
	if (value < min) {
		return min;
	}
	else if (value > max) {
		return max;
	}
	return value;
}



// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new unsigned char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}


}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new unsigned char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new unsigned char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}




// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify()
{

	// TO DO by student
	
	// sample operation
	for ( int i=0; i<Width*Height; i++ )
	{
		Data[3*i] = 0;
		Data[3*i+1] = 0;

	}

	return false;
}


//My functions!
bool MyImage::UpscaleNearestNeighbor(int targetWidth, int targetHeight) {
	// Allocate memory for the upscaled image
	unsigned char* upscaledData = new unsigned char[targetWidth * targetHeight * 3];

	// Calculate scale factors
	float xScale = static_cast<float>(Width) / targetWidth;
	float yScale = static_cast<float>(Height) / targetHeight;

	// Loop through the target image dimensions
	for (int y = 0; y < targetHeight; ++y) {
		for (int x = 0; x < targetWidth; ++x) {
			// Determine the nearest pixel in the original image
			int origX = static_cast<int>(x * xScale);
			int origY = static_cast<int>(y * yScale);

			// Clamp the indices to ensure they are within bounds
			origX = min(origX, Width - 1);
			origY = min(origY, Height - 1);

			// Get the index for the original image data
			int origIndex = (origY * Width + origX) * 3;

			// Set the upscaled image data
			int upscaledIndex = (y * targetWidth + x) * 3;
			upscaledData[upscaledIndex] = Data[origIndex];     // Blue
			upscaledData[upscaledIndex + 1] = Data[origIndex + 1]; // Green
			upscaledData[upscaledIndex + 2] = Data[origIndex + 2]; // Red
		}
	}

	// Delete the old data and update to the new upscaled data
	delete[] Data;
	Data = upscaledData;
	Width = targetWidth;
	Height = targetHeight;

	return true;
}

bool MyImage::UpscaleBilinear(int targetWidth, int targetHeight) {
	// Create a new data buffer for the upscaled image
	unsigned char* newData = new unsigned char[targetWidth * targetHeight * 3]; // Using unsigned char for RGB
	if (!newData) {
		return false; // Allocation failed
	}

	// Calculate scaling factors
	float xRatio = static_cast<float>(Width) / targetWidth;
	float yRatio = static_cast<float>(Height) / targetHeight;

	for (int y = 0; y < targetHeight; ++y) {
		for (int x = 0; x < targetWidth; ++x) {
			// Find the position in the original image
			float gx = x * xRatio;
			float gy = y * yRatio;

			// Get the coordinates of the surrounding pixels
			int gxi = static_cast<int>(floor(gx));
			int gyi = static_cast<int>(floor(gy));
			int gxi1 = min(gxi + 1, Width - 1);
			int gyi1 = min(gyi + 1, Height - 1);

			// Calculate the weights for bilinear interpolation
			float xDiff = gx - gxi;
			float yDiff = gy - gyi;

			// Get the pixel values from the original image using reinterpret_cast
			unsigned char* topLeft = reinterpret_cast<unsigned char*>(&Data[(gyi * Width + gxi) * 3]);
			unsigned char* topRight = reinterpret_cast<unsigned char*>(&Data[(gyi * Width + gxi1) * 3]);
			unsigned char* bottomLeft = reinterpret_cast<unsigned char*>(&Data[(gyi1 * Width + gxi) * 3]);
			unsigned char* bottomRight = reinterpret_cast<unsigned char*>(&Data[(gyi1 * Width + gxi1) * 3]);

			// Interpolate the color values
			for (int channel = 0; channel < 3; ++channel) {
				float top = (1 - xDiff) * topLeft[channel] + xDiff * topRight[channel];
				float bottom = (1 - xDiff) * bottomLeft[channel] + xDiff * bottomRight[channel];
				float finalValue = (1 - yDiff) * top + yDiff * bottom;

				// Clamp the final value to [0, 255]
				newData[(y * targetWidth + x) * 3 + channel] = static_cast<unsigned char>(clamp(finalValue, 0.0f, 255.0f));
			}
		}
	}

	// Clean up old data and replace with new data
	delete[] Data;
	Data = newData;
	Width = targetWidth;
	Height = targetHeight;

	return true; // Upscaling successful
}

bool MyImage::DownsampleSpecificPixel(int targetWidth, int targetHeight, const char* samplingPosition)
{
	// Ensure target dimensions are smaller than original dimensions
	if (targetWidth > Width || targetHeight > Height) {
		printf("Error: Target dimensions must be smaller than original dimensions.\n");
		return false;
	}

	// Calculate scaling ratios
	float scaleX = static_cast<float>(Width) / static_cast<float>(targetWidth);
	float scaleY = static_cast<float>(Height) / static_cast<float>(targetHeight);

	// Allocate memory for new downsampled data
	unsigned char* newData = new unsigned char[targetWidth * targetHeight * 3];

	// Define the pixel offset based on the specified sampling position
	int pixelOffsetX = 0, pixelOffsetY = 0;
	if (strcmp(samplingPosition, "top-left") == 0) {
		pixelOffsetX = 0; pixelOffsetY = 0;
	}
	else if (strcmp(samplingPosition, "center") == 0) {
		pixelOffsetX = static_cast<int>(scaleX / 2);
		pixelOffsetY = static_cast<int>(scaleY / 2);
	}
	// Add more sampling strategies here if needed
	else {
		printf("Error: Unsupported sampling position. Use 'top-left' or 'center'.\n");
		return false;
	}

	// Loop over each pixel in the downsampled image
	for (int y = 0; y < targetHeight; y++) {
		for (int x = 0; x < targetWidth; x++) {
			// Calculate the corresponding pixel in the original image
			int origX = static_cast<int>(x * scaleX) + pixelOffsetX;
			int origY = static_cast<int>(y * scaleY) + pixelOffsetY;

			// Ensure the original coordinates are within bounds
			origX = min(origX, Width - 1);
			origY = min(origY, Height - 1);

			// Copy the RGB values from the original image to the new image
			newData[(y * targetWidth + x) * 3 + 0] = Data[(origY * Width + origX) * 3 + 0]; // R
			newData[(y * targetWidth + x) * 3 + 1] = Data[(origY * Width + origX) * 3 + 1]; // G
			newData[(y * targetWidth + x) * 3 + 2] = Data[(origY * Width + origX) * 3 + 2]; // B
		}
	}

	// Update the image data with the new downsampled data
	delete[] Data;
	Data = newData;
	Width = targetWidth;
	Height = targetHeight;

	return true;
}

bool MyImage::DownsampleAverage(int targetWidth, int targetHeight)
{
	// Ensure target dimensions are smaller than original dimensions
	if (targetWidth > Width || targetHeight > Height) {
		printf("Error: Target dimensions must be smaller than original dimensions.\n");
		return false;
	}

	// Calculate scaling ratios
	float scaleX = static_cast<float>(Width) / static_cast<float>(targetWidth);
	float scaleY = static_cast<float>(Height) / static_cast<float>(targetHeight);

	// Allocate memory for new downsampled data
	unsigned char* newData = new unsigned char[targetWidth * targetHeight * 3];

	// Loop over each pixel in the downsampled image
	for (int y = 0; y < targetHeight; y++) {
		for (int x = 0; x < targetWidth; x++) {
			// Determine the region in the original image that corresponds to this downsampled pixel
			int startX = static_cast<int>(x * scaleX);
			int startY = static_cast<int>(y * scaleY);
			int endX = static_cast<int>((x + 1) * scaleX);
			int endY = static_cast<int>((y + 1) * scaleY);

			// Accumulate pixel values in the region
			int rSum = 0, gSum = 0, bSum = 0;
			int count = 0;
			for (int origY = startY; origY < endY && origY < Height; origY++) {
				for (int origX = startX; origX < endX && origX < Width; origX++) {
					rSum += Data[(origY * Width + origX) * 3 + 0];
					gSum += Data[(origY * Width + origX) * 3 + 1];
					bSum += Data[(origY * Width + origX) * 3 + 2];
					count++;
				}
			}

			// Calculate average and assign to new image
			newData[(y * targetWidth + x) * 3 + 0] = rSum / count;
			newData[(y * targetWidth + x) * 3 + 1] = gSum / count;
			newData[(y * targetWidth + x) * 3 + 2] = bSum / count;
		}
	}

	// Update the image data with the new downsampled data
	delete[] Data;
	Data = newData;
	Width = targetWidth;
	Height = targetHeight;

	return true;
}
