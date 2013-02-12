#pragma once

#include "setup_photos_dataset.h"

using namespace cv;
using namespace std;
namespace testing{
	double calculate_accuracy(const char* sketches_csv_file, const char* photos_csv_file,
									int L, int R, int P,int threshold);
}
