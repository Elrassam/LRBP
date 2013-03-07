#include "haog.h"

Mat haog::haog_hist(Mat magnitude, Mat phs){
	Mat hist = Mat::zeros(1, 9, TYPE);
	for(int x = 0; x < magnitude.rows; x++) {
		for(int y = 0; y < magnitude.cols; y++) {
			hist.at<DATA_TYPE>(0, ((phs.at<DATA_TYPE>(x,y)) / 45)) += 
				magnitude.at<unsigned char>(x,y);
		}
	}
	return hist;
}

Mat haog::get_hist_haog(Mat img){

	Mat filter_x, filter_y, gradient_x, gradient_y, gradient_x_square, gradient_y_square,
		x_y_mul, gsx, gsy, mgntud, phs, org_mgntud, org_phs;
	getDerivKernels(filter_x, filter_y, 1, 1, 1, true, TYPE);
	filter_x = filter_x.t();
	filter2D(img, gradient_x, -1, filter_x, Point(-1,-1), 0, BORDER_REFLECT101);
	filter2D(img, gradient_y, -1, filter_y, Point(-1,-1), 0, BORDER_REFLECT101);
	gradient_x.convertTo(gradient_x, TYPE);
	gradient_y.convertTo(gradient_y, TYPE);


	//magnitude(gradient_x, gradient_y, org_mgntud);
	//phase(gradient_x, gradient_y, org_phs, true);

	pow(gradient_x, 2, gradient_x_square);
	pow(gradient_y, 2, gradient_y_square);
	subtract(gradient_x_square, gradient_y_square, gsx);
	gsx.convertTo(gsx, TYPE);
	multiply(gradient_x, gradient_y, x_y_mul);
	multiply(x_y_mul, 2, gsy);
	gsy.convertTo(gsy, TYPE);
	magnitude(gsx, gsy, mgntud);
	phase(gsx, gsy, phs, true);

	/*normalize(org_mgntud, org_mgntud, 0, 255, NORM_MINMAX, CV_8UC1);
	normalize(org_phs, org_phs, 0, 360, NORM_MINMAX, TYPE);*/
	normalize(mgntud, mgntud, 0, 255, NORM_MINMAX, CV_8UC1);
	normalize(phs, phs, 0, 360, NORM_MINMAX, TYPE);
	//Mat hist = haog_hist(org_mgntud, org_phs);
	Mat hist = haog_hist(mgntud, phs);
	return hist;
	//return org_mgntud;
}

//
//
//int main(){
////
////	Mat filter_x, filter_y, gradient_x, gradient_y, gradient_x_square, gradient_y_square,
////		x_y_mul, gsx, gsy, mgntud, phs, org_mgntud, org_phs;
//	Mat img = imread("D:\\1.jpg", 1);
//	Size s(300, 300);
//	resize(img, img, s);
//	get_hist_haog(img);
//
//
//
////	getDerivKernels(filter_x, filter_y, 1, 1, 1, true, CV_32F);
////	filter_x = filter_x.t();
////	filter2D(img, gradient_x, -1, filter_x, Point(-1,-1), 0, BORDER_REFLECT101);
////	filter2D(img, gradient_y, -1, filter_y, Point(-1,-1), 0, BORDER_REFLECT101);
////	gradient_x.convertTo(gradient_x, TYPE);
////	gradient_y.convertTo(gradient_y, TYPE);
////	magnitude(gradient_x, gradient_y, org_mgntud);
////	phase(gradient_x, gradient_y, org_phs, true);
////
////	pow(gradient_x, 2, gradient_x_square);
////	pow(gradient_y, 2, gradient_y_square);
////	subtract(gradient_x_square, gradient_y_square, gsx);
////	gsx.convertTo(gsx, TYPE);
////	multiply(gradient_x, gradient_y, x_y_mul);
////	multiply(x_y_mul, 2, gsy);
////	gsy.convertTo(gsy, TYPE);
////	magnitude(gsx, gsy, mgntud);
////	phase(gsx, gsy, phs, false);
////	normalize(org_mgntud, org_mgntud, 0, 255, NORM_MINMAX, CV_8UC1);
////	normalize(mgntud, mgntud, 0, 255, NORM_MINMAX, CV_8UC1);
////	normalize(gradient_x, gradient_x, 0, 255, NORM_MINMAX, CV_8UC1);
////	normalize(gradient_y, gradient_y, 0, 255, NORM_MINMAX, CV_8UC1);
////	normalize(org_phs, org_phs, 0, 360, NORM_MINMAX, TYPE);
////	
////	//imshow("1", org_mgntud);
////	//imshow("2", mgntud);
////	//imshow("3", org_phs);
////	//imshow("4", phs);
////	//imshow("5", gradient_x);
////	//imshow("6", gradient_y);
////	for(int i = 100; i < 150; i++){
////		for(int j = 100; j < 150; j++){
////			//if(org_phs.at<DATA_TYPE>(i,j) < 0)
////				cout << org_phs.at<DATA_TYPE>(i,j)<< endl;
////		}
////	}
////	while(true);
////	waitKey(0);
//	return 0;
//}
