#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include "histogram.h"
using namespace cv;
using namespace std;


    int kernel_size2=21;
    int pos_sigma2= 1;
    int pos_lm2 = 53;
    int pos_th2 = 76;
    int pos_psi2 = 90;
    Mat src_f2,src2,dest2,image2;

    cv::Mat mkKernel(int ks, double sig, double th, double lm, double ps)
    {
        int hks = (ks-1)/2;
        double theta = th*CV_PI/180;
        double psi = ps*CV_PI/180;
        double del = 2.0/(ks-1);
        double lmbd = lm;
        double sigma = sig/ks;
        double x_theta;
        double y_theta;
        cv::Mat kernel(ks,ks, CV_32F);
        for (int y=-hks; y<=hks; y++)
        {
            for (int x=-hks; x<=hks; x++)
            {
                x_theta = x*del*cos(theta)+y*del*sin(theta);
                y_theta = -x*del*sin(theta)+y*del*cos(theta);
                kernel.at<float>(hks+y,hks+x) = (float)exp(-0.5*(pow(x_theta,2)+pow(y_theta,2))/pow(sigma,2))* cos(2*CV_PI*x_theta/lmbd + psi);
            }
        }
        return kernel;
    }

    void Process(int , void *)
	{
        double sig = pos_sigma2;
		//double lm = pos_lm2; 
        double lm = 0.5+pos_lm2/100.0;
        double th = pos_th2;
        double ps = pos_psi2;
        cv::Mat kernel = mkKernel(kernel_size2, sig, th, lm, ps);
        cv::filter2D(src2, dest2, CV_32F, kernel);
        cv::imshow("Process window", dest2);
		imwrite("D:\\im2.png", dest2);
     /*   cv::Mat Lkernel(kernel_size2*20, kernel_size2*20, CV_32F);
        cv::resize(kernel, Lkernel, Lkernel.size());
        Lkernel /= 2.;
        Lkernel += 0.5;*/
        //cv::imshow("Kernel", Lkernel);

        //cv::Mat mag;
        //cv::pow(dest2, 2.0, mag);
        //cv::imshow("Mag", mag);
	  
    }

    int mainuuu(int argc, char** argv)
    {
		cv::Mat image2 = cv::imread("D:\\012.jpg",1);

        //cv::imshow("Image", image2);
        
        cv::cvtColor(image2, src2, CV_BGR2GRAY);
		Size s(300, 300);
		resize(src2, src2, s);
		//cv::imshow("src", src);
        //src.convertTo(src_f2, CV_32F, 1.0/255, 0);
		//cv::imshow("Src_f", src_f2);
       /* if (!kernel_size2%2)
        {
            kernel_size2+=1;
        }*/
        cv::namedWindow("Process window", 1);
        cv::createTrackbar("Sigma", "Process window", &pos_sigma2, kernel_size2, Process);
        cv::createTrackbar("Lambda", "Process window", &pos_lm2, 100, Process);
        cv::createTrackbar("Theta", "Process window", &pos_th2, 180, Process);
        cv::createTrackbar("Psi", "Process window", &pos_psi2, 360, Process);
        Process(0,0);

		cout<<"Done" << endl;
		
        cv::waitKey(0);
        return 0;
    }
