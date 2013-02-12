#pragma once

#include <iostream>
#include <stdio.h>
#include "lbp.h"
#include "histogram.h"
#include "MatlabEngine.h"

using namespace cv;
using namespace std;

namespace pyrmd_rep {
	Mat preprocessing(Mat image);
	void radon_transform(Mat& cell, Mat& dst, CMatlabEngine& mt);
	void divide_image_into_cells(vector<Mat> &cells, Mat & image);
	void apply_lbp(Mat &image, Mat& lbp, int R, int P);
	Mat read_image(const char * file_name);
	void concatenate_histograms(vector<Mat>& cells_hist, Mat &image_hist);
	vector<Mat> generate_hists_for_each_level(const char* file_name, int L, int R, int P, CMatlabEngine &mt);
	double calculate_dist(vector<Mat> & levels_hists1, vector<Mat> & levels_hists2);
	Mat cropImage(IplImage *img, CvRect region);
}
