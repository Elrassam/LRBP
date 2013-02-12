#include "testing.h"

double testing::calculate_accuracy(const char* sketches_csv_file, const char* photos_csv_file,
									int L, int R, int P, int threshold) {

	CoInitialize(NULL);
	CMatlabEngine mt;
	int counter = 0;
	int sketch_number = 1;

	if (mt.IsInitialized()){
		mt.Show(false);
		mt.Execute("figure('visible', 'off');addpath('D:\\Work space\\c++ vs2010\\Hello_OpenCV\\LRBP\\exportfig');");
		ifstream sketches_file(sketches_csv_file, ifstream::in);
		string sk_line, sk_path, sk_classlabel, pho_path;
		char separator = ',';
		while (getline(sketches_file, sk_line)) {
		
			cout << sketch_number << endl;
			sk_path = sk_classlabel = "";
			stringstream sk_line_ss(sk_line);
			getline(sk_line_ss, sk_path, separator);
			getline(sk_line_ss, sk_classlabel);

			vector<Mat> levels_hist1 = pyrmd_rep::generate_hists_for_each_level(sk_path.c_str(), L, 
																						R, P, mt);
			vector<double> vec_dists(threshold);
			vector<string> vec_selected_paths(threshold);
			fill (vec_dists.begin(),vec_dists.end(),1.7976931348623157e+308);
			fill (vec_selected_paths.begin(),vec_selected_paths.end(),"");
			ifstream photos_file(photos_csv_file, ifstream::in);

			while (getline(photos_file, pho_path)) {

				vector<Mat> levels_hist2(L);
				setup_photos_dataset::load_hists_for_each_level(levels_hist2, pho_path, L, P);

				double dist = pyrmd_rep::calculate_dist(levels_hist1, levels_hist2);
			
				if(dist < vec_dists[threshold - 1]) {
					vec_dists[threshold - 1] = dist;
					vec_selected_paths[threshold - 1] = pho_path;
					for (int i = 0; i < threshold; i++) {
						for (int j = i + 1; j < threshold; j++) {
							if (vec_dists[j] < vec_dists[i]) {
								double tmp = vec_dists[i];
								vec_dists[i] = vec_dists[j];
								vec_dists[j] = tmp;
								string str_tmp = vec_selected_paths[i];
								vec_selected_paths[i] = vec_selected_paths[j];
								vec_selected_paths[j] = str_tmp;
							}
						}
					}
				}
			}
			sort (vec_selected_paths.begin(), vec_selected_paths.end());
			if(binary_search(vec_selected_paths.begin(), vec_selected_paths.end(), sk_classlabel)){
				counter++;
			}
			cout << ((counter * 100.0) / sketch_number) << endl;
			sketch_number++;
	   }
		mt.Quit();
		CoUninitialize();
	}
	return ((counter * 100.0) / 188.0);
}
