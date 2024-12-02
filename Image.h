//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Parag Havaldar
// Main Image class structure 
//
//*****************************************************************************

#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <cstring> // for memcpy
#include <cmath>   // for floor and ceil


// Class structure of Image 
// Use to encapsulate an RGB image
class MyImage 
{

private:
	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location
	unsigned char*	Data;					// RGB data of the image

public:
	// Constructor
	MyImage();
	// Copy Constructor
	MyImage::MyImage( MyImage *otherImage);
	// Destructor
	~MyImage();

	// operator overload
	MyImage & operator= (const MyImage & otherImage);

	// Reader & Writer functions
	void	setWidth( const int w)  { Width = w; }; 
	void	setHeight(const int h) { Height = h; }; 
	void	setImageData( const char *img ) { Data = (unsigned char *)img; };
	void	setImagePath( const char *path) { strcpy(ImagePath, path); }
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	unsigned char*	getImageData() { return Data; };
	char*	getImagePath() { return ImagePath; }

	// Input Output operations
	bool	ReadImage();
	bool	WriteImage();

	// Modifications
	bool	Modify();

	//My Functions!
	bool MyImage::UpscaleNearestNeighbor(int targetWidth, int targetHeight);
	bool MyImage::UpscaleBilinear(int targetWidth, int targetHeight);
	bool MyImage::DownsampleSpecificPixel(int targetWidth, int targetHeight, const char* samplingPosition);
	bool MyImage::DownsampleAverage(int targetWidth, int targetHeight);

	bool MyImage::DownsampleGaussian(int targetWidth, int targetHeight, double sigma);

};

#endif //IMAGE_DISPLAY
