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
    int chan = img.channels();
    std::cout << "# of channels: " << chan << "\n";
    if(chan == 3){
        cv::Mat different_channels[3];
        cv::split(img, different_channels);
        
        imshow("Blue Channel",different_channels[0]);
        imshow("Green Channel",different_channels[1]);
        imshow("Red Channel",different_channels[2]);
        
    }

    char Key = (char)cv::waitKey(0);
    std::cout << "Out Key: " << Key << "\n";

    return 0;
}