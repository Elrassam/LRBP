#include "testing.h"

double testing::calculate_accuracy(const char* sketches_csv_file, const char* photos_csv_file,
									int L, int R, int P, int threshold) {

	CoInitialize(NULL);
	CMatlabEngine mt;
	int counter = 0;
	int sketch_number = 1;
	ofstream outfile;
	outfile.open ("D:\\result_tests.txt");

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


			/*unsigned found_slash = sk_path.find_last_of("\\");
			unsigned dot = sk_path.find_last_of(".");
			string file_name = sk_path.substr(found_slash + 1, dot - found_slash - 1);
			string folder_name = sk_path.substr(0, found_slash + 1);*/
			bool flag = false;

			//for(int patch = 1; patch <= 3 && !flag; patch++){
				//stringstream ss_sk;
				//ss_sk << folder_name << file_name /*<< "_" << patch*/ << ".jpg";
				//string sk_path_patch = ss_sk.str();

				vector<Mat> levels_hist1_with_gabor = pyrmd_rep::generate_hists_for_each_level(sk_path/*_patch*/.c_str(),
																		L, R, P, mt, true);
				//vector<Mat> levels_hist1_without_gabor = pyrmd_rep::generate_hists_for_each_level(sk_path_patch.c_str(),
					//													L, R, P, mt, false);
				vector<double> vec_dists(threshold);
				vector<string> vec_selected_paths(threshold);
				fill (vec_dists.begin(),vec_dists.end(),1.7976931348623157e+308);
				fill (vec_selected_paths.begin(),vec_selected_paths.end(),"");
				ifstream photos_file(photos_csv_file, ifstream::in);

				while (getline(photos_file, pho_path)) {

					//unsigned found_slash_pho = pho_path.find_last_of("\\");
					//unsigned dot_pho = pho_path.find_last_of(".");
					//string file_name_pho = pho_path.substr(found_slash_pho + 1, dot_pho - found_slash_pho - 1);
					//string folder_name_pho = pho_path.substr(0, found_slash_pho + 1);
					//stringstream ss_ph;
					//ss_ph << folder_name_pho << file_name_pho << "_" << patch << ".jpg";
					//string ph_path_patch = ss_ph.str();

					vector<Mat> levels_hist2_with_gabor(L);//, levels_hist2_without_gabor(L);
					setup_photos_dataset::load_hists_for_each_level(levels_hist2_with_gabor,
													/*levels_hist2_without_gabor,*/ /*ph_path_patch*/ pho_path, L, P);

					double dist = pyrmd_rep::calculate_dist(levels_hist1_with_gabor, levels_hist2_with_gabor);
					//dist += pyrmd_rep::calculate_dist(levels_hist1_without_gabor, levels_hist2_without_gabor);
			
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
					flag = true;
				}
				for(int i = 0; i < vec_selected_paths.size(); i++){
					outfile << vec_selected_paths[i] << "\n";
					cout << vec_selected_paths[i] << endl;
				}
			//}
			outfile << ((counter * 100.0) / sketch_number) << "    " << flag << "\n";
			outfile << "=============================================" << "\n";

			cout << ((counter * 100.0) / sketch_number) << "    " << flag << endl;
			cout << "=============================================" << endl;
			sketch_number++;
	   }
		mt.Quit();
		CoUninitialize();
	}
	outfile.close();
	return ((counter * 100.0) / 50.0);
}
