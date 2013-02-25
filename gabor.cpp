#include "gabor.h"


Mat gabor::mkKernel(int ks, double sig, double th, double lm, double ps) {
    int hks = (ks-1)/2;
    double theta = th*CV_PI/180;
    double psi = ps*CV_PI/180;
	double del = 2.0/(ks-1);
	double lmbd = lm;
	double sigma = sig/ks;
	double x_theta;
	double y_theta;
	Mat kernel(ks, ks, CV_32F);
	for (int y=-hks; y<=hks; y++)
	{
		for (int x=-hks; x<=hks; x++)
		{
			x_theta = x*del*cos(theta)+y*del*sin(theta);
			y_theta = -x*del*sin(theta)+y*del*cos(theta);
			kernel.at<float>(hks+y,hks+x) = (float)exp(-0.5*(pow(x_theta,2)+pow(y_theta,2))/pow(sigma,2))* cos(2*CV_PI*x_theta/lmbd + psi);
		}
	}
	return kernel;
}

Mat gabor::gabor(Mat src_f) {
	int kernel_size = 21;
	double sig = 2.5;
	double lm = 1.5;
	double th = 24;
	double ps = 90;
	Mat kernel = mkKernel(kernel_size, sig, th, lm, ps);
	Mat dest;
	filter2D(src_f, dest, CV_32F, kernel);

	Mat Lkernel(kernel_size*20, kernel_size*20, CV_32F);
	resize(kernel, Lkernel, Lkernel.size());
	Lkernel /= 2.;
	Lkernel += 0.5;

	Mat mag;
	pow(dest, 2.0, mag);
	return mag;
}