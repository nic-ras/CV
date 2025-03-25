#include <opencv2/highgui.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    if(argc <  2)
    {
        std::cout << "An image filename shall be provided\n";
        return -1;
    }
    cv::Mat img = cv::imread(argv[1]);
    if(img.empty())
    {
        std::cout << "A correct image directory shall be provided\n";
        return -1;
    }
    cv::namedWindow("Example 1");
    cv::imshow("Example 1", img);
    cv::waitKey(0);

    return 0;
}

