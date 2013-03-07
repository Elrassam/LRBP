#pragma once

#include <cv.h>
#include <limits>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

#define TYPE CV_32FC1
#define DATA_TYPE float

namespace haog{
	Mat haog_hist(Mat magnitude, Mat phs);
	Mat get_hist_haog(Mat img);
}