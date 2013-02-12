#pragma once

#include <cv.h>
#include <limits>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

namespace histogram {
	template <typename _Tp>
	void histogram_(const Mat& src, Mat& hist, int numPatterns);
	template <typename _Tp>
	double chi_square_(const Mat& histogram0, const Mat& histogram1);
	void histogram(const Mat& src, Mat& hist, int numPatterns);
	double chi_square(const Mat& histogram0, const Mat& histogram1);
}

