#pragma once
#include "testing.h"

int main() {
    int R = 8;
    int P = 8;
	int L = 4;
	int threshold = 5;
	
	/*CoInitialize(NULL);
	CMatlabEngine mt;
	if (mt.IsInitialized()){
		mt.Show(false);
		mt.Execute("figure('Visible','off');addpath('D:\\Work space\\c++ vs2010\\Hello_OpenCV\\LRBP\\exportfig');");
		setup_photos_dataset::save_hists_for_each_level("D:\\ph_III.csv", L, R, P, "LEVELS\\", mt);
		mt.Quit();
		CoUninitialize();
	}*/
	
	
	double acc = testing::calculate_accuracy("D:\\sk_III.csv","D:\\ph_new_III.csv",L, R, P, threshold);
	return 0; 
}
