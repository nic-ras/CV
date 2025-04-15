#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

cv::Mat img, img_gray, dst;
std::vector<cv::Vec3f> circles;

const char* window_name = "windows";

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
    cv::blur(img_gray, img_gray, cv::Size(5, 5)); 

    cv::HoughCircles(img_gray, circles, cv::HOUGH_GRADIENT, 1,
            img_gray.rows/16,  
            100, 30, 1, 30 
        );
    
    cv::Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
    int radius = cvRound(circles[0][2]);
    cv::circle( img, center, radius, cv::Scalar(0,255,0), 5, cv::LINE_AA);

    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::imshow( window_name, img );
    
    cv::waitKey(0);

    return 0;
}  

