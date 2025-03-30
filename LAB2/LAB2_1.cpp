#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
int main()
{

    cv::Mat img = cv::imread("/home/roaming/rasenic69592/Desktop/CV/LAB2/Garden.jpg");

    if(img.empty())
    {
        std::cout << "A correct image directory shall be provided\n";
        return -1;
    }

    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    cv::imwrite("Garden_grayscale.jpg", img_gray);

    cv::namedWindow("Garden grayscale", cv::WINDOW_AUTOSIZE);
    cv::imshow("Garden grayscale", img_gray);
    
    cv::waitKey(0);

    return 0;
}  

