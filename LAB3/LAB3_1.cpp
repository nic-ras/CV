#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

void BGR_triplet(int event, int x, int y, int flags, void* img){
    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::Mat* image = static_cast<cv::Mat*>(img); 
        cv::Vec3b bgr = image->at<cv::Vec3b>(y, x);
        std::cout << "Click at (" << x << ", " << y << "): "
                  << "B = " << (int)bgr[0] << ", "
                  << "G = " << (int)bgr[1] << ", "
                  << "R = " << (int)bgr[2] << std::endl;
    }
} 

int main()
{
    cv::Mat img = cv::imread("/home/roaming/rasenic69592/Desktop/CV/LAB3/Robocup.jpg");

    if(img.empty())
    {
        std::cout << "A correct image directory shall be provided\n";
        return -1;
    }


    cv::namedWindow("window");
    cv::imshow("window",img);
    cv::setMouseCallback("window", BGR_triplet, &img);
    
    cv::waitKey(0);

    return 0;
}  

