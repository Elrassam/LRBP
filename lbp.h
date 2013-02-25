#pragma once

#include <cv.h>
#include <limits>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

namespace lbp {
	template <typename _Tp>
	void ELBP_(const cv::Mat& src, cv::Mat& dst, int radius = 1, int neighbors = 8);
	void ELBP(const Mat& src, Mat& dst, int radius = 1, int neighbors = 8);
	template <typename _Tp>
	void VARLBP_(const Mat& src, Mat& dst, int radius, int neighbors = 8);
	void VARLBP(const Mat& src, Mat& dst, int radius, int neighbors = 8);
}
