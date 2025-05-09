#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

std::vector<std::string> getImagePathsFromPattern(const std::string& pattern);
std::vector<cv::Mat> loadImages(const std::vector<std::string>& imagePaths);

#endif // IMAGE_LOADER_H