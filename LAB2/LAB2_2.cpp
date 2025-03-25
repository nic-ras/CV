#include "ImageFilters.h"

int main()
{
    cv::Mat img = cv::imread("/home/raseranico/Desktop/CV/LAB2/Garden.jpg");

    if(img.empty())
    {
        std::cout << "A correct image directory shall be provided\n";
        return -1;
    }

    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    cv::imwrite("Garden_grayscale.jpg", img_gray);

    cv::Mat min_fil = ImageFilters::min(img_gray, 5);
    cv::Mat max_fil = ImageFilters::max(img_gray, 5);

    cv::namedWindow("Garden grayscale", cv::WINDOW_AUTOSIZE);
    cv::imshow("Garden grayscale", img_gray);
    
    cv::waitKey(0);

    return 0;
}