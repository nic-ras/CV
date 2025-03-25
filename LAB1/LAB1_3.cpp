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

    cv::namedWindow("Example pre");
    cv::imshow("Example pre", img);
    int chan = img.channels();
    std::cout << "# of channels: " << chan << "\n";
    if(chan == 3){
        for (int i = 0; i < img.rows; i++) {
            for (int x = 0; x < img.cols; x++) {
             img.at<cv::Vec3b>(i,x)[2] = 0;
            }
        }
        cv::namedWindow("Example Post");
        cv::imshow("Example Post", img);

    }
    char Key = (char)cv::waitKey(0);
    std::cout << "Out Key: " << Key << "\n";

    return 0;
}