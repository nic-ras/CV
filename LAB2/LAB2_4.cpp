#include "ImageFilters.h"

int main()
{
    cv::Mat img = cv::imread("/home/roaming/rasenic69592/Desktop/CV/LAB2/Astronaut_salt_pepper.png");

    if(img.empty())
    {
        std::cout << "A correct image directory shall be provided\n";
        return -1;
    }

    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    cv::Mat gau_img;
    gau_fil(img_gray, gau_img, 5);

    cv::namedWindow("Grayscale original image", cv::WINDOW_NORMAL);
    cv::imshow("Grayscale original image", img_gray);


    cv::namedWindow("Gaussian filter", cv::WINDOW_NORMAL);
    cv::imshow("Gaussian filter", gau_img);
    
    
    cv::waitKey(0);

    return 0;
}