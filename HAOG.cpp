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

int main(){

	Mat filter_x, filter_y, gradient_x, gradient_y, gradient_x_square, gradient_y_square,
		x_y_mul, gsx, gsy, mgntud, phs, org_mgntud, org_phs;

	Mat img = imread("D:\\1.jpg", 1);
	Size s(300, 300);
	resize(img, img, s);

	
	getDerivKernels(filter_x, filter_y, 1, 1, 1, true, CV_32F);
	filter_x = filter_x.t();

	filter2D(img, gradient_x, -1, filter_x, Point(-1,-1), 0, BORDER_REFLECT101);
	filter2D(img, gradient_y, -1, filter_y, Point(-1,-1), 0, BORDER_REFLECT101);
	gradient_x.convertTo(gradient_x, TYPE);
	gradient_y.convertTo(gradient_y, TYPE);

	magnitude(gradient_x, gradient_y, org_mgntud);
	phase(gradient_x, gradient_y, org_phs, false);

	pow(gradient_x, 2, gradient_x_square);
	pow(gradient_y, 2, gradient_y_square);
	subtract(gradient_x_square, gradient_y_square, gsx);
	gsx.convertTo(gsx, TYPE);

	multiply(gradient_x, gradient_y, x_y_mul);
	multiply(x_y_mul, 2, gsy);
	gsy.convertTo(gsy, TYPE);

	magnitude(gsx, gsy, mgntud);
	phase(gsx, gsy, phs, false);

	org_mgntud.convertTo(org_mgntud, CV_8U);
	mgntud.convertTo(mgntud, CV_8U);
	org_phs.convertTo(org_phs, CV_8U);
	phs.convertTo(phs, CV_8U);
	gradient_x.convertTo(gradient_x, CV_8U);
	gradient_y.convertTo(gradient_y, CV_8U);

	imshow("1", org_mgntud);
	imshow("2", mgntud);
	//imshow("3", org_phs);
	//imshow("4", phs);
	//imshow("5", gradient_x);
	//imshow("6", gradient_y);

	waitKey(0);
	return 0;
}