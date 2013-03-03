#include "setup_photos_dataset.h"

void setup_photos_dataset::save_hists_for_each_level(const char* photos_csv_file, int L, int R,
											int P, string sub_dir, CMatlabEngine &mt){

	ifstream photos_file(photos_csv_file, ifstream::in);
	string pho_path;
	int counter = 1;
	while (getline(photos_file, pho_path)) {
		cout << counter << "\\" << 63 << endl;
		counter++;
		unsigned found = pho_path.find_last_of("\\");
		unsigned dot = pho_path.find_last_of(".");
		string file_name = pho_path.substr(found + 1, dot - found - 1);
		string folder_name = pho_path.substr(0, found + 1);

		for(int patch = 1; patch <= 3; patch++){
			stringstream ss_lbp;
			ss_lbp << folder_name << sub_dir << file_name << "_" << patch << ".lbp";
			stringstream ss_pho_patch;
			ss_pho_patch << folder_name << file_name << "_" << patch << ".jpg";

			string pho_path_patch = ss_pho_patch.str();

			vector<Mat> levels_hist1 = pyrmd_rep::generate_hists_for_each_level(pho_path_patch.c_str(), L, R, P, mt, true);
			vector<Mat> levels_hist2 = pyrmd_rep::generate_hists_for_each_level(pho_path_patch.c_str(), L, R, P, mt, false);
			ofstream myfile;
			myfile.open ( ss_lbp.str());
			for (int l = 0; l < L; l++) {
				for(int y = 0; y < levels_hist1[l].cols; y++) {
					myfile << levels_hist1[l].at<double>(0, y) << ((y + 1 < levels_hist1[l].cols)? "," : "");
				}
				myfile << "\n";
				for(int y = 0; y < levels_hist2[l].cols; y++) {
					myfile << levels_hist2[l].at<double>(0, y) << ((y + 1 < levels_hist2[l].cols)? "," : "");
				}
				(l + 1 < L)? myfile << "\n": myfile << "";
			}
			myfile.close();
		}
	}
}

void setup_photos_dataset::load_hists_for_each_level(vector<Mat> &levels_hist1, vector<Mat> &levels_hist2
										, string pho_path, int L, int P){
	unsigned dot = pho_path.find_last_of(".");
	string path_name = pho_path.substr(0, dot);
	path_name += ".lbp";
	string hist_values_line;
	ifstream hist_file(path_name, ifstream::in);
	int i = 0, num_bins = (int)(floor(pow(2.0, P))),cells_num = 0;
	bool first = true;
	while (getline(hist_file, hist_values_line)) {
		char * pch = strtok(((char*)hist_values_line.c_str()), ",");
		int col = 0;
		cells_num = (int)(floor(pow(2.0, i * 2)));
		if(first){
			levels_hist1[i] = Mat::zeros(1, 8 * cells_num, CV_64FC1);
			while (pch != NULL){
				levels_hist1[i].at<double>(0, col++) = atoi(pch);
				pch = strtok (NULL, ",");
			}
			first = false;
		} else {
			levels_hist2[i] = Mat::zeros(1, num_bins * cells_num, CV_64FC1);
			while (pch != NULL){
				levels_hist2[i].at<double>(0, col++) = atoi(pch);
				pch = strtok (NULL, ",");
			}
			first = true;
			i++;
		}
	}
}
