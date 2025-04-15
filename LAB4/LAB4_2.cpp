#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

cv::Mat img;
int white = 255;
int black = 0;
int T = 10;
std::vector<cv::Vec2f> lines;

const char* window_name = "windows";

int main()
{
    img = cv::imread("/home/roaming/rasenic69592/Desktop/CV/LAB4/street_scene.png");

    if(img.empty())
    {
        std::cout << "A correct image directory shall be provided\n";
        return -1;
    }
    
    cv::Mat mask(img.size(), CV_8UC1, cv::Scalar(0));

    for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols; j++){
            cv::Vec3b pixel = img.at<cv::Vec3b>(i, j);
            int dist_b = abs(pixel[0] - (int)255);
            int dist_g = abs(pixel[1] - (int)255);
            int dist_r = abs(pixel[2] - (int)255);

            if(dist_b < T && dist_g < T && dist_r < T){
                mask.at<uchar>(i,j) = white;
            }
            else{
                mask.at<uchar>(i,j) = black;
            }

        }
    } 

    cv::medianBlur( mask, mask, 3 );

    cv::HoughLines(mask, lines, 5, CV_PI/180, 150, 0, 0 );

    float rho1 = lines[0][0], theta1 = lines[0][1];
    cv::Point pt1, pt2;
    double a = cos(theta1), b = sin(theta1);
    double x0_1 = a*rho1, y0_1 = b*rho1;
    pt1.x = cvRound(x0_1 - 1*(-b));
    pt1.y = cvRound(y0_1 - 1*(a));
    pt2.x = cvRound(x0_1 - 320*(-b));
    pt2.y = cvRound(y0_1 - 320*(a));

    float rho2 = lines[4][0], theta2 = lines[4][1];
    cv::Point pt3, pt4;
    double c = cos(theta2), d = sin(theta2); 
    double x0_2 = c*rho2, y0_2 = d*rho2;
    pt3.x = cvRound(x0_2 - 560*(-d));
    pt3.y = cvRound(y0_2 - 560*(c));
    pt4.x = cvRound(x0_2 - 1000*(-d));
    pt4.y = cvRound(y0_2 - 1000*(c));
    
    std::vector<cv::Point> polygon = {pt1, pt2, pt3, pt4};
    cv::fillConvexPoly(img, polygon, cv::Scalar(0,0,255));
    cv::line(img, pt1, pt2, cv::Scalar(255, 0, 0), 2); // blu
    cv::line(img, pt3, pt4, cv::Scalar(255, 0, 0), 2); // blu

    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::imshow( window_name, img );
    
    cv::waitKey(0);

    return 0;
}  

