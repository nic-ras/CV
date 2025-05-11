#ifndef CYLINDRICAL_PROJECTION_H
#define CYLINDRICAL_PROJECTION_H

#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

std::vector<cv::Mat> CylindricalProjection(std::vector<cv::Mat> images, double angle);

#endif // CYLINDRICAL_PROJECTION_H