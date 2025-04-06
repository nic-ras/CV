#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

void BGR_triplet(int event, int x, int y, int flags, void* img){
    if (event == cv::EVENT_LBUTTONDOWN) {

        cv::Mat* image = static_cast<cv::Mat*>(img);

        if (x >= 4 && x < image->cols - 4 && y >= 4 && y < image->rows - 4) {
             
            cv::Mat kernel = cv::Mat::ones(9, 9, CV_32F) / 81.0f; 
            cv::Rect roi(x - 4, y - 4, 9, 9);
            cv::Mat element = (*image)(roi);

            cv::Mat b_channel;
            cv::Mat g_channel;
            cv::Mat r_channel;
            cv::extractChannel(element, b_channel, 0); 
            cv::extractChannel(element, g_channel, 1); 
            cv::extractChannel(element, r_channel, 2); 

            cv::Scalar sample(cv::mean(b_channel)[0], cv::mean(g_channel)[0], cv::mean(r_channel)[0]);
            std::cout << "Click at (" << x << ", " << y << "): "
                  << "B mean = " << sample[0] << ", "
                  << "G mean = " << sample[1] << ", "
                  << "R mean = " << sample[2] << std::endl;
        

            cv::Mat mask(image->size(), CV_8UC1, cv::Scalar(0));
            int white = 255;
            int black = 0;
            int T = 80;

            for (int i = 0; i < image->rows; i++){
                for (int j = 0; j < image->cols; j++){
                    cv::Vec3b pixel = image->at<cv::Vec3b>(i, j);
                    int dist_b = abs(pixel[0] - (int)sample[0]);
                    int dist_g = abs(pixel[1] - (int)sample[1]);
                    int dist_r = abs(pixel[2] - (int)sample[2]);

                    if(dist_b < T && dist_g < T && dist_r < T){
                        mask.at<uchar>(i,j) = white;
                    }
                    else{
                        mask.at<uchar>(i,j) = black;
                    }

                }
            }   

            cv::imshow("Mask", mask);
    
        }
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


    cv::namedWindow("window", cv::WINDOW_NORMAL);
    cv::imshow("window",img);
    cv::setMouseCallback("window", BGR_triplet, &img);
    
    cv::waitKey(0);

    return 0;
}  

