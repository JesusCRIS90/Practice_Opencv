#pragma once

#include "Header.h"

namespace examples{

	struct Data
	{
		pcv::InfoTrackbar t;
		cv::Mat out;
		cv::Mat in;
	};

	void Example1();
	void Example2();
	void Example3();
	void Example4();
	void Example5();	// Using of the differents fixed and otsu thresholding
	void Example6();	// Adaptative thresholding --> Useful for augmented reality markers
	void Example7();	// Using the Smoothing functions
	void Example8();	// Using Sobel function
	void Example9();	// Use of Erosion and Erode				
	void Example10();	// Use the generic function to morphology transformations --> morphologyEx()	(TUTORIAL OPENCV DOCUMENTATIONS)
	void Example11();	// Use the morphology operations HIT-MASS			(TUTORIAL OPENCV DOCUMENTATIONS)
	void Example12();	// Threshold with video
	void Example13();	// Use function LAPLACIAN							(TUTORIAL OPENCV DOCUMENTATIONS)
	void Example14();	// Use morphology operations with music partitures	(TUTORIAL OPENCV DOCUMENTATIONS)
	void Example15();	// Circle coins
	void Example16();	// Example code of Course Edx Introduction to opencv
	void Example17();	// Drawing the histogram of an image				(TUTORIAL OPENCV DOCUMENTATIONS)
	void Example18();	// Example code of Correlation --> Course Edx Introduction to opencv
	void Example19();	// Use of the class Constrast and Trackbar to change the contrast ogf the image
	void Example20();	// Use the class ImageManager with Video


	void VerifyGetClassContrast(pcv::Contrast&);
	void VerifySetClassContrast(pcv::Contrast&);
	
	void VerifyGetClassContrast(pcv::ImageManager*);
	void VerifySetClassContrast(pcv::ImageManager*);

	void Threshold(int, void*);
	void Erosion(int, void*);
	void Dilation(int, void*);
	void Morphology_Operations(int, void*);
	
	void on_low_r_thresh_trackbar(int, void *);
	void on_high_r_thresh_trackbar(int, void *);
	void on_low_g_thresh_trackbar(int, void *);
	void on_high_g_thresh_trackbar(int, void *);
	void on_low_b_thresh_trackbar(int, void *);
	void on_high_b_thresh_trackbar(int, void *);

	void show_wait_destroy(const char* winname, cv::Mat img);

}

