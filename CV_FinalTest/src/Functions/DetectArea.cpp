/************************************************************
 *  Project : Document Scanner 
 *  File    : DetectArea.cpp
 *  Author  : Riccardo Grosso
 *  Course  : Computer Vision 
 *  Teacher : Stefano Ghidoni
 ************************************************************/
#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"

using namespace cv;

cv::Mat drawHoughLines(const cv::Mat& image, std::vector<cv::Vec3f> lines, cv::Scalar color);
cv::Point findIntersection(cv::Vec3f line1, cv::Vec3f line2);
int findCorners(std::vector<std::vector<cv::Point>> clustered_points, std::vector<cv::Point>* corners);


typedef struct
{
    int lines;
    int points;
    std::vector<Point> point_vec;
} output;


int DetectArea(const cv::Mat& image, std::vector<cv::Point>* coordinates)
{
    int i, j;
    float canny_increment1 = 1.3, canny_increment2 = 1.2;
    std::vector<int> hough_thresholds = {50, 75, 100, 125, 150};

    //Preprocess (Gray conversion + Gaussian Blur)
    Mat gray, blurred, preprocessed, edges, lines_image;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    int sigmaBlur = 3;
    GaussianBlur(gray, blurred, Size(0,0), sigmaBlur);
    blurred.copyTo(preprocessed);

    //Variables for parameters refining
    int trial = 0;
    int use_previous = 0;
    int to_continue = 0;

    std::vector<Point> intersection_points;
    int num_points;
    int num_lines;
    output previous_res;

    //Initial Canny thresholds
    int thrCanny1 = 30;
    int thrCanny2 = 20;
    double rhoHough, thetaHough;
    int thrHough;

    do
    {
        use_previous = 0;
        intersection_points.clear();
        std::vector<Vec3f> hlines;
        std::vector<Vec3f> vlines;

        //Setting Canny thresholds
        thrCanny1 = static_cast<int>(thrCanny1*canny_increment1);
        thrCanny2 = static_cast<int>(thrCanny2*canny_increment2);
        //Accelerating the converge speed for too low thresholds
        if((previous_res.points>10000 || to_continue==1)  && trial>0)
        {
            int increment_speed = 2-trial;
            if(increment_speed <= 0)
            {
                increment_speed = 1;
            }
            thrCanny1 = static_cast<int>(thrCanny1*increment_speed*canny_increment1);
            thrCanny2 = static_cast<int>(thrCanny2*increment_speed*canny_increment2);
            to_continue = 0;
        }
        //Find edges (Canny)
        Canny(preprocessed, edges, thrCanny1, thrCanny2);

        //Find lines (Hough)
        std::vector<Vec3f> lines;
        rhoHough = 1;

        //Loop for getting the best pair (theta,threshold) for the HoughLines alg.
        j = 0;
        do
        {
            thrHough = hough_thresholds[j];     //Setting the Houghlines threshold

            //While lines are too many, change thetaHough
            i = 0;
            thetaHough = 0;
            do{
                i++;
                thetaHough += CV_PI/180;
                lines.clear();
                HoughLines(edges, lines, rhoHough, thetaHough, thrHough);
                
                //Dividing into horizontal and vertical lines
                hlines.clear();
                vlines.clear();    
                for(int k=0; k<lines.size(); k++)
                {
                    //Dividing horizontal and vertical, rejecting others
                    double angle = lines[k][1]*180/CV_PI;
                    if(angle < 15 || angle > 165)
                    {
                        vlines.push_back(lines[k]);
                    }
                    if(angle > 75 && angle < 105)
                    {
                        hlines.push_back(lines[k]);
                    }
                }
                num_lines = vlines.size() + hlines.size();
            }while(num_lines > 10000 && i < 2);
            j++;
        }while(j < hough_thresholds.size() && num_lines > 10000);

        if(num_lines<10000 || trial==0)
        {
            //Drawing lines on image
            cvtColor(edges, lines_image, COLOR_GRAY2BGR);
            lines_image = drawHoughLines(lines_image, hlines, Scalar(0,0,255));
            lines_image = drawHoughLines(lines_image, vlines, Scalar(0,255,0));

            if(lines_image.empty())     //Image is empty if there are no lines to draw
            {
                use_previous = 1;
                num_lines = 0;
                num_points = 0;
            }
            else
            {
                //Find intersection points between vertical and horizontal lines
                Point pt;
                for(i=0; i<hlines.size(); i++)
                {
                    for(j=0; j<vlines.size(); j++)
                    {
                        pt = findIntersection(hlines[i], vlines[j]);

                        //Checking if it's a valid point (different from (-1,-1) and inside the image)
                        if(pt.x >= 0 && pt.y >= 0 && pt.x < lines_image.cols && pt.y < lines_image.rows)
                        {
                            //Checking if it's not in the center
                            float ylower_bound = lines_image.rows/3.0;
                            float yupper_bound = lines_image.rows/3.0*2;
                            float xlower_bound = lines_image.cols/3.0;
                            float xupper_bound = lines_image.cols/3.0*2;
                            if((pt.y < ylower_bound || pt.y > yupper_bound) && (pt.x < xlower_bound || pt.x > xupper_bound))
                            {
                                intersection_points.push_back(pt);
                            }
                        }
                    }
                }
                num_points = intersection_points.size();
            }
        }
        else
        {
            to_continue = 1;
        }

        //Checking if previous result was better
        if(trial>0 && num_lines<previous_res.lines*0.5 && (num_points<previous_res.points*0.5 || num_points==0) || use_previous==1)
        {
            //Exit the loop
            use_previous = 1;
        }
        else
        {
            //Save new result
            previous_res.point_vec.clear();
            previous_res.lines = num_lines;
            previous_res.points = num_points;
            //Copying intersection pts to save them
            for(int k=0; k<num_points; k++)
            {
                previous_res.point_vec.push_back(intersection_points[k]);
            }

        }

        trial++;
    }while( (num_points>3.5*num_lines || to_continue==1) && use_previous==0 && std::max(thrCanny1,thrCanny2)<200 && trial < 7);

    if(use_previous == 1)
    {
        //Recover last result
        num_lines = previous_res.lines;
        num_points = previous_res.points;
        intersection_points.clear();
        for(int k=0; k<num_points; k++)
        {
            intersection_points.push_back(previous_res.point_vec[k]);
        }
    }

    //Grouping the intersection points
    std::vector<std::vector<Point>> clustered_points;
    clustered_points.clear();
    clustered_points.resize(4);
    //Grouping based on region of image
    float lower_bound = 1.0/3, upper_bound = 2.0/3;
    for(i=0; i<num_points; i++)
    {
        float ylower_bound = image.rows*lower_bound;
        float yupper_bound = image.rows*upper_bound;
        float xlower_bound = image.cols*lower_bound;
        float xupper_bound = image.cols*upper_bound;
        Point pt = intersection_points[i];
        if(pt.x<xlower_bound && pt.y<ylower_bound)      //Top-left region
        {
            clustered_points[0].push_back(pt);
        }
        if(pt.x>xupper_bound && pt.y<ylower_bound)      //Top-right region
        {
            clustered_points[1].push_back(pt);
        }
        if(pt.x>xupper_bound && pt.y>yupper_bound)      //Bottom-right region
        {
            clustered_points[2].push_back(pt);
        }
        if(pt.x<xlower_bound && pt.y>yupper_bound)      //Bottom-left region
        {
            clustered_points[3].push_back(pt);
        }
    }

    //Selecting corners
    std::vector<Point> corners;
    findCorners(clustered_points, &corners);

    //Returning the 4 corners
    for(i=0; i<corners.size(); i++)
    {
        coordinates->push_back(corners[i]);     
    }
    if(coordinates->size()!=4)
    {
        return -1;
    }

    return 0;
}

cv::Mat drawHoughLines(const cv::Mat& image, std::vector<cv::Vec3f> lines, cv::Scalar color)
{
    int dim;

    //Security check
    if(lines.size() <= 0)
    {
        return Mat();
    }

    //Choosing highest dimension
    dim = std::max(image.rows,image.cols);

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + dim*(-b));
        pt1.y = cvRound(y0 + dim*(a));
        pt2.x = cvRound(x0 - dim*(-b));
        pt2.y = cvRound(y0 - dim*(a));
        line(image, pt1, pt2, color, 1, LINE_AA);
    }
    
    return image;
}

cv::Point findIntersection(cv::Vec3f line1, cv::Vec3f line2)
{   
    Point pt;
    float r1 = line1[0], r2 = line2[0];
    float th1 = line1[1], th2 = line2[1];
    float costh1 = cosf(th1);     
    float sinth1 = sinf(th1);     
    float costh2 = cosf(th2);     
    float sinth2 = sinf(th2);     
    float det = sinth2*costh1 - sinth1*costh2;
    if(det != 0.0)
    {
        pt.y = static_cast<int>((r2*costh1 - r1*costh2)/det);
        pt.x = static_cast<int>((r1*sinth2 - r2*sinth1)/det);
    }
    else
    {
        //lines are parallel
        pt.x = -1;
        pt.y = -1;
    }
    return pt;
}

int findCorners(std::vector<std::vector<cv::Point>> clustered_points, std::vector<cv::Point>* corners)
{
    //For each cluster
    for(int i=0; i<4; i++)
    {
        int n_points = clustered_points[i].size();
        if(n_points<=0)
        {
            std::cerr << "No points in the cluster" << std::endl;
        }

        //Extract y coordinates
        std::vector<int> y_values;
        for(int j=0; j<n_points; j++)
        {
            y_values.push_back(clustered_points[i][j].y);
        }

        //Sort y
        std::sort(y_values.begin(), y_values.end());

        //Choose y
        int y_chosen;
        if(i<2)    //If belongs to upper part of image (first 2 clusters)
        {
            y_chosen = y_values[n_points/100];     //Choose first percentile
        }
        else       //If belongs to lower part of image (lattest 2 clusters)
        {
            y_chosen = y_values[n_points*99/100];   //Choose 99-th percentile
        }

        //Choose x
        int x_chosen;
        //Extract x
        std::vector<int> x_values;
        for(int j=0; j<n_points; j++)
        {
            x_values.push_back(clustered_points[i][j].x);
        }
        
        //Choosing min or max value
        if(i==0 || i==3)    //If belongs to left part of image (first and last cluster)
        {
            x_chosen = *std::min_element(x_values.begin(), x_values.end());     //Choose min
        }
        else            //If belongs to right part of image (middle 2 clusters)
        {
            x_chosen = *std::max_element(x_values.begin(), x_values.end());     //Choose max
        }
        Point pt(x_chosen, y_chosen);
        corners->push_back(pt);
    }

    return 0;
}
