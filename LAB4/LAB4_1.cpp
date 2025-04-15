#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

cv::Mat img, img_gray;
cv::Mat dst, detected_edges;

int lowThreshold = 0;
const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 3;
const char* window_name = "windows";

static void CannyThreshold(int, void*)
{
    cv::medianBlur( img_gray, detected_edges, 3);
    cv::Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    dst = cv::Scalar::all(0);
    img.copyTo( dst, detected_edges);
    cv::imshow( window_name, dst );
}

int main()
{
    img = cv::imread("/home/roaming/rasenic69592/Desktop/CV/LAB4/street_scene.png");

    if(img.empty())
    {
        std::cout << "A correct image directory shall be provided\n";
        return -1;
    }

    dst.create( img.size(), img.type() );
    cv::cvtColor( img, img_gray, cv::COLOR_BGR2GRAY );

    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
    CannyThreshold(0, 0);
    
    cv::waitKey(0);

    return 0;
}  

