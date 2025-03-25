#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

// Min filter
int min_fil( cv::Mat& original, cv::Mat& output, int k );

// Max filter
int max_fil( cv::Mat& original, cv::Mat& output, int k );

// Median filter
int med_fil( cv::Mat& original, cv::Mat& output, int k );

// Gaussian filter
int gau_fil( cv::Mat& original, cv::Mat& output, int k );
    

 
    