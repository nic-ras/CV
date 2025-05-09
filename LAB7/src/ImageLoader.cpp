#include "ImageLoader.h"
#include <opencv2/core/utils/filesystem.hpp>
#include <iostream>

std::vector<std::string> getImagePathsFromPattern(const std::string& pattern) {
    std::vector<cv::String> cvPaths;
    cv::utils::fs::glob(pattern, cvPaths, false);

    std::vector<std::string> paths(cvPaths.begin(), cvPaths.end());
    return paths;
}

std::vector<cv::Mat> loadImages(const std::vector<std::string>& imagePaths) {
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