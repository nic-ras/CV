#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    if(argc <  2)
    {
        std::cout << "An image filename shall be provided\n";
        return -1;
    }

    cv::Mat img_gray = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

    if(img_gray.empty())
    {
        std::cout << "A correct image directory shall be provided\n";
        return -1;
    }

    // Averaging Filter
    cv::Mat avg_fil;
    cv::blur( img_gray, avg_fil, cv::Size( 5, 5 ) );

    // Sobel filter
    cv::Mat sob_fil;
    cv::Sobel(img_gray, sob_fil, CV_64F, 1, 0, 1);

    // Min filter
    cv::Mat kernel = cv::Mat::ones(2, 2, CV_8U);
    cv::Mat min_fil;
    cv::erode(img_gray, min_fil, kernel);

    // Max filter
    cv::Mat max_fil;
    cv::dilate(img_gray, max_fil, kernel);

    
    cv::namedWindow("Original");
    cv::imshow("Original", img_gray);
    cv::namedWindow("Averaging Filter");
    cv::imshow("Averaging Filter", avg_fil);
    cv::namedWindow("Sobel Filter");
    cv::imshow("Sobel Filter", sob_fil);
    cv::namedWindow("Min Filter");
    cv::imshow("Min Filter", min_fil);
    cv::namedWindow("Max Filter");
    cv::imshow("Max Filter", max_fil);
    
    cv::waitKey(0);

    return 0;
}