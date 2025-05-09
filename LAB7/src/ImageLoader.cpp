#include "ImageLoader.h"
#include <opencv2/core/utils/filesystem.hpp>
#include <iostream>

std::vector<cv::String> getImagePathsFromPattern(const cv::String& pattern) {
    const std::vector<cv::String> cvPaths;
    cv::utils::fs::glob(pattern, cvPaths, false);

    std::vector<cv::String> paths(cvPaths.begin(), cvPaths.end());
    return paths;
}

std::vector<cv::Mat> loadImages(const std::vector<cv::String>& imagePaths) {
    std::vector<cv::Mat> images;
    for (const auto& path : imagePaths) {
        cv::Mat img = cv::imread(path);
        if (img.empty()) {
            std::cerr << "Could not load image: " << path << std::endl;
        } else {
            images.push_back(img);
        }
    }
    return images;
}