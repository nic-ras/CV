#include "ImageFilters.h"

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

    cv::Mat min_img;
    min_fil(img_gray, min_img, 5);
    cv::Mat max_img;
    max_fil(img_gray, max_img, 5);

    cv::namedWindow("Grayscale original image", cv::WINDOW_NORMAL);
    cv::imshow("Grayscale original image", img_gray);

    cv::namedWindow("Min filter", cv::WINDOW_NORMAL);
    cv::imshow("Min filter", min_img);

    cv::namedWindow("Max filter", cv::WINDOW_NORMAL);
    cv::imshow("Max filter", max_img);
    
    
    cv::waitKey(0);

    return 0;
}