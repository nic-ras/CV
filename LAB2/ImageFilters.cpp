#include "ImageFilters.h"

// Min filter
int min_fil( cv::Mat& original, cv::Mat& output, int k ) 
{
    if(original.empty() || ( (int)(k) == k && k <= 0 )){
        std::cout << "Parametri non corretti\n";
        return -1;
    }

    cv::Mat kernel = cv::Mat::ones(k, k, CV_8U);

    cv::erode(original, output, kernel);

    return 1;
}

// Max filter
int max_fil( cv::Mat& original, cv::Mat& output, int k )
{
    if(original.empty() || ( (int)(k) == k && k <= 0 )){
        std::cout << "Parametri non corretti\n";
        return -1;
    }
    
    cv::Mat kernel = cv::Mat::ones(k, k, CV_8U);

    cv::dilate(original, output, kernel);

    return 1;
}

// Median filter
int med_fil( cv::Mat& original, cv::Mat& output, int k )
{
    if(original.empty() || ( (int)(k) == k && k <= 0 )){
        std::cout << "Parametri non corretti\n";
        return -1;
    }

    cv::medianBlur( original, output,  k);

    return 1;
}

// Gaussian filter
int gau_fil( cv::Mat& original, cv::Mat& output, int k )
{
    if(original.empty() || ( (int)(k) == k && k <= 0 )){
        std::cout << "Parametri non corretti\n";
        return -1;
    }

    cv:: GaussianBlur( original, output,  cv::Size(k, k), 0);

    return 1;
}