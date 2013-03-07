#include "histogram.h"

template <typename _Tp>
void histogram::histogram_(const Mat& src, Mat& hist, int numPatterns, bool gabor_flag) {
	hist = Mat::zeros(1, numPatterns, CV_64FC1);
	for(int x = 0; x < src.rows; x++) {
		for(int y = 0; y < src.cols; y++) {
			if(gabor_flag){
				hist.at<double>(0,((src.at<_Tp>(x,y)) / 32)) += 1;
			} else {
				hist.at<double>(0,src.at<_Tp>(x,y)) += 1;
			}
		}
	}
}

template <typename _Tp>
double histogram::chi_square_(const Mat& histogram0, const Mat& histogram1) {
	Mat dst = histogram1.clone();
	double result = 0.0;
	if(histogram0.type() != histogram1.type())
		CV_Error(CV_StsBadArg, "Histograms must be of equal types.");
	if(histogram0.rows != 1 || histogram0.rows != dst.rows || histogram0.cols != dst.cols){
		CV_Error(CV_StsBadArg, "Histograms must be of equal dimension.");
	}
	for(int i=0; i < histogram0.cols; i++) {
		double a = histogram0.at<_Tp>(0,i) - dst.at<_Tp>(0,i);
		double b = histogram0.at<_Tp>(0,i) + dst.at<_Tp>(0,i);
		if(abs(b) > numeric_limits<double>::epsilon()) {
			result+=(a*a)/b;
		}
}
	
	return result;
}

double histogram::chi_square(const Mat& histogram0, const Mat& histogram1) {
	switch(histogram0.type()) {
		case CV_8SC1: return chi_square_<char>(histogram0,histogram1); break;
		case CV_8UC1: return chi_square_<unsigned char>(histogram0,histogram1); break;
		case CV_16SC1: return chi_square_<short>(histogram0, histogram1); break;
		case CV_16UC1: return chi_square_<unsigned short>(histogram0,histogram1); break;
		case CV_32SC1: return chi_square_<int>(histogram0,histogram1); break;
		case CV_64FC1: return chi_square_<double>(histogram0,histogram1); break;
		case CV_32FC1: return chi_square_<float>(histogram0,histogram1); break;
	}
}

void histogram::histogram(const Mat& src, Mat& hist, int numPatterns, bool gabor_flag) {
	switch(src.type()) {
		case CV_8SC1: histogram_<char>(src, hist, numPatterns, gabor_flag); break;
		case CV_8UC1: histogram_<unsigned char>(src, hist, numPatterns, gabor_flag); break;
		case CV_16SC1: histogram_<short>(src, hist, numPatterns, gabor_flag); break;
		case CV_16UC1: histogram_<unsigned short>(src, hist, numPatterns, gabor_flag); break;
		case CV_32SC1: histogram_<int>(src, hist, numPatterns, gabor_flag); break;
		case CV_64FC1: histogram_<double>(src, hist, numPatterns, gabor_flag); break;
	}
}
