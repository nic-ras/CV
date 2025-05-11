#ifndef FIND_TRANSLATION_H
#define FIND_TRANSLATION_H

#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

std::vector<cv::Point2f> FindTranslation(std::vector<std::vector<cv::KeyPoint>> allKeypoints, 
    std::vector<std::vector<cv::DMatch>> refinedMatches, 
    double ratio);

#endif // FIND_TRANSLATION_H