#include "lbp.h"

template <typename _Tp>
void lbp::ELBP_(const Mat& src, Mat& dst, int radius, int neighbors) {
	float M_PI = 3.14f;
	neighbors = max(min(neighbors,31),1); // set bounds...
	dst = Mat::zeros(src.rows-2*radius, src.cols-2*radius, CV_32SC1);
	for(int n=0; n<neighbors; n++) {
		// sample points
		float x = static_cast<float>(radius) * cos(2.0f*M_PI*n/static_cast<float>(neighbors));
		float y = static_cast<float>(radius) * -sin(2.0f*M_PI*n/static_cast<float>(neighbors));
		// relative indices
		int fx = static_cast<int>(floor(x));
		int fy = static_cast<int>(floor(y));
		int cx = static_cast<int>(ceil(x));
		int cy = static_cast<int>(ceil(y));
		// fractional part
		float ty = y - fy;
		float tx = x - fx;
		// set interpolation weights
		float w1 = (1 - tx) * (1 - ty);
		float w2 =      tx  * (1 - ty);
		float w3 = (1 - tx) *      ty;
		float w4 =      tx  *      ty;
		// iterate through your data
		for(int i=radius; i < src.rows-radius;i++) {
			for(int j=radius;j < src.cols-radius;j++) {
				float t = (float)(w1*src.at<_Tp>(i+fy,j+fx) + w2*src.at<_Tp>(i+fy,j+cx) + w3*src.at<_Tp>(i+cy,j+fx) + w4*src.at<_Tp>(i+cy,j+cx));
				// we are dealing with floating point precision, so add some little tolerance
				dst.at<unsigned int>(i-radius,j-radius) += ((t > src.at<_Tp>(i,j)) && (abs(t-src.at<_Tp>(i,j)) > std::numeric_limits<float>::epsilon())) << n;
			}
		}
	}
}


void lbp::ELBP(const Mat& src, Mat& dst, int radius, int neighbors) {
	switch(src.type()) {
		case CV_8SC1: ELBP_<char>(src, dst, radius, neighbors); break;
		case CV_8UC1: ELBP_<unsigned char>(src, dst, radius, neighbors); break;
		case CV_16SC1: ELBP_<short>(src, dst, radius, neighbors); break;
		case CV_16UC1: ELBP_<unsigned short>(src, dst, radius, neighbors); break;
		case CV_32SC1: ELBP_<int>(src, dst, radius, neighbors); break;
		case CV_32FC1: ELBP_<float>(src, dst, radius, neighbors); break;
		case CV_64FC1: ELBP_<double>(src, dst, radius, neighbors); break;
	}
}