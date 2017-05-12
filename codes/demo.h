/*
* This file was used to show the representation of ftc algorithm.
* Here we provided some functions to use ftc.
* Author: Jin Huang
* Date: 2017/05/11
*/

#include "HistList.h"

// parameters
typedef struct FTCParams
{
	double eps;
	double sigma;
	int width;
	bool bThird;
	bool bSmooth;
	float*image;
	size_t nx, ny, nc;
}PARAMS;

// For hue histogram.
// Set all the values in "img" that between "s1" and "s2" 
// to their mean value.
void SettoMean_Hue(float*img, float *hImg, 
	int rows, int cols, int s1, int s2);

// For gray histogram.
// Set all the values in "img" that between "s1" and "s2" 
// to their mean value.
void SettoMean_Gray(float*img, int rows, int cols, int s1, int s2);

// Get the hue img from RGB.
float* GetHueFromRGB(float* img, int rows, int cols);

// For hue histogram.
// Get the histogram.
void CalculateHistFromHue(CHistList &hist, float* img, int rows, int cols);

// For gray histogram.
// Get the histogram.
void CalculateHistFromGray(CHistList &hist, float* img, int rows, int cols);

// get the gaussian kernel list
void GetGaussianKernel(CHistList& list,int width,double sigma);

// Smooth the list using Gaussian function ,
// the bandwidth will be set by user and defalt is 3
void Smooth(CHistList& list, int width, double sigma);

// Get the image from 'io_png' and the control parameters.
void ftc_image_from_io_png(PARAMS params);
