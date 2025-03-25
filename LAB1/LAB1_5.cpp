#include <opencv2/highgui.hpp>

int main()
{
    cv::Mat imgA(256, 256, CV_8UC1);
    cv::Mat imgB(256, 256, CV_8UC1);

    cv::Mat imgC(300, 300, CV_8UC1, cv::Scalar(255));
    cv::Mat imgD(300, 300, CV_8UC1, cv::Scalar(255));

    for(int i = 0; i < imgA.rows; i++){
        for(int x = 0; x < imgA.cols; x++){
            imgA.at<uchar>(i,x) = (uchar)i;
        }
    }

    for(int i = 0; i < imgB.rows; i++){
        for(int x = 0; x < imgB.cols; x++){
            imgB.at<uchar>(i,x) = (uchar)x;
        }
    }

    for (int y = 0; y < imgC.rows; y++) {
        for (int x = 0; x < imgC.cols; x++) {
            if (((x / 20) % 2) ^ ((y / 20) % 2)) {
                imgC.at<uchar>(y, x) = 0;  
            }
        }
    }

    for (int y = 0; y < imgD.rows; y++) {
        for (int x = 0; x < imgD.cols; x++) {
            if (((x / 50) % 2) ^ ((y / 50) % 2)) {
                imgD.at<uchar>(y, x) = 0;  
            }
        }
    }

    cv::namedWindow("Vertical Gradient", cv::WINDOW_AUTOSIZE);
    cv::imshow("Vertical Gradient", imgA);

    cv::namedWindow("Horizontal Gradient", cv::WINDOW_AUTOSIZE);
    cv::imshow("Horizontal Gradient", imgB);

    cv::namedWindow("Chessboard 20px", cv::WINDOW_AUTOSIZE);
    cv::imshow("Chessboard 20px", imgC);

    cv::namedWindow("Chessboard 50px", cv::WINDOW_AUTOSIZE);
    cv::imshow("Chessboard 50px", imgD);

    cv::waitKey(0);
    
    return 0;
}