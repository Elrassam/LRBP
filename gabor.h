#pragma once

#include <cv.h>
#include <limits>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

namespace gabor{
	Mat gabor(Mat src_f);
	Mat mkKernel(int ks, double sig, double th, double lm, double ps);
}