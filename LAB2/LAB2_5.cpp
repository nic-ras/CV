#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
int main()
{
    int bins = 256;
    int histSize[] = {bins};
    float range[] = { 0, 256 };
    const float* histRange[] = { range };
    

    cv::Mat img = cv::imread("/home/roaming/rasenic69592/Desktop/CV/LAB2/Garden.jpg");

    if(img.empty())
    {
        std::cout << "A correct image directory shall be provided\n";
        return -1;
    }

    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

    cv::Mat hist;
    cv::calcHist( &img_gray, 1, 0, cv::Mat(), hist, 1, histSize, histRange);

    int hist_w = 512, hist_h = 400; 
    int bin_w = cvRound((double)hist_w / bins);
    cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );

    cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX);

    for (int i = 1; i < bins; i++) {
        cv::line(histImage,
                 cv::Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
                 cv::Point(bin_w * i, hist_h - cvRound(hist.at<float>(i))),
                 cv::Scalar(255, 255, 255), 2, 8, 0);
    }

    cv::namedWindow("Garden histogram", cv::WINDOW_AUTOSIZE);
    cv::imshow("Garden histogram", histImage);
    
    cv::waitKey(0);

    return 0;
}  

