#pragma once

#include <fstream>
#include "pyramid_representation.h"
using namespace cv;
using namespace std;

namespace setup_photos_dataset {
	void save_hists_for_each_level(const char* photos_csv_file, int L, int R, int P, string sub_dir, CMatlabEngine &mt);
	void load_hists_for_each_level(vector<Mat> &levels_hist1/*, vector<Mat> &levels_hist2*/, string pho_path, int L, int P);
}

