#ifndef MATCH_FEATURES_H
#define MATCH_FEATURES_H

#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

std::vector<std::vector<cv::DMatch>> MatchFeatures(std::vector<cv::Mat> allDescriptors);

#endif // MATCH_FEATURES_H