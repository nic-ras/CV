#ifndef REFINE_MATCHES_H
#define REFINE_MATCHES_H

#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

std::vector<std::vector<cv::DMatch>> RefineMatches(std::vector<std::vector<cv::DMatch>> allMatches,  double ratio);

#endif // REFINE_MATCHES_H