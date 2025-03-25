#include "ImageFilters.h"

// Min filter
int min( &original, &output, int k ) 
{
    if(*original.empty() || ( (int)(k) == k && k <= 0 )){
        std::cout << "Parametri non corretti\n";
        return -1;
    }

    cv::Mat kernel = cv::Mat::ones(k, k, CV_8U);

    cv::erode(*original, *output, kernel);

    return 1;
}

// Max filter
cv::Mat min( &original, &output, int k )
{
    if(*original.empty() || ( (int)(k) == k && k <= 0 )){
        std::cout << "Parametri non corretti\n";
        return -1;
    }
    
    cv::Mat kernel = cv::Mat::ones(k, k, CV_8U);

    cv::dilate(*original, *output, kernel);

    return max_fil;
}