/*
 * Copyright (c) 2017-2018, Jin Huang <jin_huang@whu.edu.cn>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under, at your option, the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version, or
 * the terms of the simplified BSD license.
 *
 * You should have received a copy of these licenses along this
 * program. If not, see <http://www.gnu.org/licenses/> and
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/**
* This file was used to show the representation of ftc algorithm.
* Here we provided some functions to use ftc.
* Author: Jin Huang
* Date: 2017/05/11
*/

#include "demo.h"
#include "ftc.h"

void GetGaussianKernel(CHistList& klist,int width,double sigma)
{
	if (width <= 0)
	{
		width = 1;
	}
	int size = 2 * width + 1;
	double dsigma = sigma*sigma;
	double PI = 3.141592653;

	klist.Init(size);
	klist.SetSize(size);
	for (int i = 0; i < size; i++)
	{
		int x = i - width;
		klist[i] = exp(-(x*x) / (2 * dsigma)) / (2 * PI*dsigma);
	}

	double sum = klist.Sum();
	for (int i = 0; i < size; i++)
	{
		klist[i] = klist[i] / sum;
	}
}

void Smooth(CHistList & list,int width,double sigma)
{
	if (2 * width + 1 > list.GetSize())
		return;

	CHistList klist;
	GetGaussianKernel(klist, width, sigma);

	int begin_index = 0;
	int end_index = list.GetSize() - 1;

	if (!list.IsCircle())
	{
		begin_index = width;
		end_index = list.GetSize() - width - 1;
	}

	// conv
	for (int i = begin_index; i <= end_index; i++)
	{
		double value = 0;
		for (int j = -width; j <= width; j++)
		{
			value += list[i + j] * klist[j + width];
		}
		list[i] = value;
	}
}

void SettoMean_Gray(float*img, int rows, int cols, int s1, int s2)
{
	double g = 0;
	int num = 0;
	int temp = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			temp = img[i*cols + j];
			if (temp >= s1 && temp < s2)
			{
				g += temp;
				num++;
			}
		}
	}
	unsigned char meang = g / num;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			temp = img[i*cols + j];
			if (temp >= s1 && temp < s2)
			{
				img[i*cols + j] = meang;
			}
		}
	}
}

void SettoMean_Hue(float*img, float *hImg, int rows, int cols, int s1, int s2)
{
	double r = 0, g = 0, b = 0;
	float h = 0;
	int num = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			h = hImg[i*cols + j];
			if (s1 > s2)
			{
				if (h >= s1 || h < s2)
				{
					r += img[i*cols + j];
					g += img[rows*cols + i*cols + j];
					b += img[rows*cols * 2 + i*cols + j];
					num++;
				}
			}
			else if (h >= s1 && h < s2)
			{
				r += img[i*cols + j];
				g += img[rows*cols + i*cols + j];
				b += img[rows*cols * 2 + i*cols + j];
				num++;

			}
		}
	}

	unsigned char meanr = r / num;
	unsigned char meang = g / num;
	unsigned char meanb = b / num;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			h = hImg[i*cols + j];
			if (s1 > s2)
			{
				if (h >= s1 || h < s2)
				{
					img[i*cols + j] = meanr;
					img[rows*cols + i*cols + j] = meang;
					img[rows*cols * 2 + i*cols + j] = meanb;
				}
			}
			else if (h >= s1 && h < s2)
			{
				img[i*cols + j] = meanr;
				img[rows*cols + i*cols + j] = meang;
				img[rows*cols * 2 + i*cols + j] = meanb;
			}
		}
	}
}

float* GetHueFromRGB(float* img, int rows, int cols)
{
	float* hImg = new float[rows*cols];
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			float min, max, delta, tmp;
			float r, g, b;
			r = img[i*cols + j];
			g = img[rows*cols + i*cols + j];
			b = img[rows*cols * 2 + i*cols + j];

			tmp = MIN(r, g);
			min = MIN(tmp, b);
			tmp = MAX(r, g);
			max = MAX(tmp, b);

			delta = max - min;

			if (delta == 0)
			{
				hImg[i*cols + j] = -1;
				continue;
			}

			if (r == max)
				tmp = (g - b) / delta; // between yellow & magenta
			else if (g == max)
				tmp = 2 + (b - r) / delta; // between cyan & yellow
			else
				tmp = 4 + (r - g) / delta; // between magenta & cyan

			tmp *= 60; // degrees
			if (tmp < 0)
				tmp += 360;

			hImg[i*cols + j] = tmp;
		}
	}

	return hImg;
}

void CalculateHistFromHue(CHistList &hist, float* img, int rows, int cols)
{
	float temp;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			temp = img[i*cols + j];
			if (temp >= 0)
			{
				temp = int(temp + 0.5);
				if (temp >= 360)
				{
					temp = temp - 360;
				}
			}
			hist[temp] += 1;
		}
	}
}

void CalculateHistFromGray(CHistList &hist, float* img, int rows, int cols)
{
	unsigned char temp;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			temp = img[i*cols + j];
			hist[temp] += 1;
		}
	}
}

void ftc_image_from_io_png(PARAMS params)
{
	int level = 0;
	CHistList src, out;
	float *hImg = NULL;

	if (params.nc == 1)
	{
		level = 256;
		src.Init(level);
		src.SetSize(level);
		CalculateHistFromGray(src, params.image, params.nx, params.ny);
	}
	else if (params.nc == 3)
	{
		level = 360;
		src.Init(level);
		src.SetSize(level);
		src.SetCircle();
		hImg = GetHueFromRGB(params.image, params.nx, params.ny);
		CalculateHistFromHue(src, hImg, params.nx, params.ny);
	}
	else
	{
		return;
	}

	// smooth the original list
	if (params.bSmooth)
	{
		Smooth(src, params.width, params.sigma);
	}

	src.SaveasTxt("histogram.txt");
	//src.Display();

	// main function of FTC algorithm
	FTCSegment(params.eps, src, out,params.bThird);
	out.SaveasTxt("index.txt");

	// Get the output image
	for (int i = 0; i < out.GetSize(); i++)
	{
		if (out.IsCircle())
		{
			if (i == out.GetSize() - 1)
			{
				SettoMean_Hue(params.image, hImg, 
					params.nx, params.ny, out[i], out[0]);
			}
			else
			{
				SettoMean_Hue(params.image, hImg,
					params.nx, params.ny, out[i], out[i + 1]);
			}
		}
		else
		{
			if (i != out.GetSize() - 1)
			{
				SettoMean_Gray(params.image,
					params.nx, params.ny, out[i], out[i + 1]);
			}
		}
	}
}