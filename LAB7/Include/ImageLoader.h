#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

std::vector<cv::String> getImagePathsFromPattern(const cv::String& directory, const cv::String& pattern);
std::vector<cv::Mat> loadImages(const std::vector<cv::String>& imagePaths);

#endif // IMAGE_LOADER_H