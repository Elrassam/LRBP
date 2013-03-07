#include "pyramid_representation.h"
#include "gabor.h"
#include "haog.h"

void pyrmd_rep::divide_image_into_cells(vector<Mat> &cells, Mat & img) {
	int cells_num = cells.size();
	int cells_num_sqr_root = (int)floor(sqrt((double)cells_num));
	int rows = (img.rows % 2 == 0)? img.rows : img.rows - 1;
	int cols = (img.cols % 2 == 0)? img.cols : img.cols - 1;
	int N_rows = rows / cells_num_sqr_root;
	int N_cols = cols / cells_num_sqr_root;
	int i = 0;
	for (int r = 0; r < rows; r += N_rows){
		for (int c = 0; c < cols; c += N_cols){
			if(i < cells_num) {
				cells[i] = img(Range(r, min(r + N_rows, rows)), Range(c, min(c + N_cols, cols))).clone();
				i++;
			}
		}
	}
}

void pyrmd_rep::apply_lbp(Mat &image, Mat& lbp, int R, int P) {
	lbp::ELBP(image, lbp, R, P);
	normalize(lbp, lbp, 0, 255, NORM_MINMAX, CV_64FC1);
}

Mat pyrmd_rep::read_image(const char * file_name) {
	Mat src = imread(file_name, 1);
	Mat image_grey, image_resized;
	cvtColor(src, image_grey, CV_BGR2GRAY);
	Size size(200, 85);
	resize(image_grey, image_resized, size);
	//image_resized = preprocessing(image);
	return image_resized;
}

void pyrmd_rep::concatenate_histograms(vector<Mat>& cells_hist, Mat &image_hist){
	image_hist = cells_hist[0];
	for(int i = 1; i < cells_hist.size(); i++) {
		hconcat(image_hist, cells_hist[i], image_hist);
	}
}

double pyrmd_rep::calculate_dist(vector<Mat> & levels_hists1, vector<Mat> & levels_hists2){
	int L = levels_hists1.size();
	double dist = histogram::chi_square(levels_hists1[0], levels_hists2[0]) / pow(2.0, L);
	for(int l = 1; l < L; l++) {
		dist += histogram::chi_square(levels_hists1[l], levels_hists2[l]) / pow(2.0, L - l + 1);
	}
	return dist;
}

void pyrmd_rep::radon_transform(Mat& cell, Mat& dst, CMatlabEngine &mt){
	if (!mt.IsInitialized()){
		cout << "matlab not initialized" << endl;
		return;
	}
	imwrite("D:\\im.jpg", cell);
	UINT nRows = cell.rows; 
	UINT nCols = cell.cols;  
	vector<double> v( nRows * nCols );
	mt.PutMatrix( ("M"), v, nRows, nCols);
	mt.Execute("[rows, cols] = size(M);B = imresize(radon(imread('D:\\im.jpg'),0:180),[rows cols]);imshow(B,[],'Colormap',hot);set(gca, 'LooseInset', get(gca, 'TightInset'));set(gca,'units','normalized','position',[0 0 1 1]);export_fig D:\\im.jpg;");
	Mat temp = imread("D:\\im.jpg", 1);
	cvtColor(temp, dst, CV_RGB2GRAY);
}

vector<Mat> pyrmd_rep::generate_hists_for_each_level(const char* file_name, int L,
															int R, int P, CMatlabEngine &mt, bool gabor_flag) {
	Mat src = read_image(file_name);
	Mat image;
	//if(gabor_flag){
		//image = gabor::gabor(src);
		//image.convertTo(image,CV_16UC1,255,0);
//	} else {
		image = src;
	//}
	//int num_bins = (int)(floor(pow(2.0, P)));
	vector<Mat> image_hists_for_each_level(L);
	for(int l = 0; l < L; l++) {
		int cells_num = (int)(floor(pow(2.0, l * 2)));
		vector<Mat> cells(cells_num);
		vector<Mat> cells_radon(cells_num);
		//vector<Mat> cells_lbp(cells_num);
		//////////added
		//vector<Mat> cells_haog(cells_num);
		/////////////
		vector<Mat> cells_hist(cells_num);
		divide_image_into_cells(cells, image);
		for(int c = 0; c < cells_num; c++) {
			radon_transform(cells[c], cells_radon[c], mt);
			/////////////////////////////// added
			//gabor_flag = false;
			//apply_lbp(cells[c], cells_lbp[c], R, P);
			cells_hist[c] = haog::get_hist_haog(cells_radon[c]);
			
			//histogram::histogram(cells_lbp[c], cells_hist[c], num_bins, gabor_flag);
			//////////////////////////////////////

			/*if(gabor_flag){
				histogram::histogram(cells_radon[c], cells_hist[c], 8, gabor_flag);
			} else {
				apply_lbp(cells_radon[c], cells_lbp[c], R, P);
				histogram::histogram(cells_lbp[c], cells_hist[c], num_bins, gabor_flag);
			}*/
		}
		concatenate_histograms(cells_hist, image_hists_for_each_level[l]);
	}
	return image_hists_for_each_level;
}

Mat pyrmd_rep::cropImage(IplImage *src, CvRect region) {
	Size si = Size(region.height,region.width);
	IplImage* cropped = cvCreateImage(si, src->depth, src->nChannels );
	cvSetImageROI( src,  region );
	cropped =cvCloneImage(src);
	cvResetImageROI( src );
	Mat cropped_mat(cropped);
	return cropped_mat;
}

Mat pyrmd_rep::preprocessing(Mat image) {
	CvRect region = cvRect(12, 12, 176, 218);
	IplImage* image_Ipl=cvCloneImage(&(IplImage)image);
	Mat cropped_image = cropImage(image_Ipl, region);
	return cropped_image;
}
