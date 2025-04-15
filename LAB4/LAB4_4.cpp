#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

cv::Mat img, mask;
int T = 75;

const char* window_name = "windows";

int main()
{
    img = cv::imread("/home/roaming/rasenic69592/Desktop/CV/LAB4/street_scene.png");

    if(img.empty())
    {
        std::cout << "A correct image directory shall be provided\n";
        return -1;
    }
    cv::cvtColor(img, mask, cv::COLOR_BGR2HSV);
    cv::Vec3b sample = mask.at<cv::Vec3b>(0, mask.cols / 2); 

    for (int i = 0; i < mask.rows; i++){
        for (int j = 0; j < mask.cols; j++){
            cv::Vec3b pixel = mask.at<cv::Vec3b>(i, j);
            int dist_h = abs(pixel[0] - (int)sample[0]);
            int dist_s = abs(pixel[1] - (int)sample[1]);
            int dist_v = abs(pixel[2] - (int)sample[2]);

            if(dist_h < T && dist_s < T && dist_v < T){
                //mask.at<uchar>(i,j) = white;
                mask.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 255, 255);   

            }

        }
    } 

    //cv::medianBlur( mask, mask, 3 );
    cv::cvtColor(mask, mask, cv::COLOR_HSV2BGR);
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::imshow( window_name, mask );
    
    cv::waitKey(0);

    return 0;
}  

