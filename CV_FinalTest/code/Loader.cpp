#include "ImageLoader.h"
#include <opencv2/core.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <iostream>

void loadDataset(const cv::String& imageDir,
                 const cv::String& pattern,
                 const cv::String& coordFile,
                 std::vector<cv::Mat>& images,
                 std::vector<std::vector<cv::Point2f>>& coords)
{
    // 1. Get the image paths
    std::vector<cv::String> cvPaths;
    cv::utils::fs::glob(imageDir, pattern, cvPaths, false);

    std::vector<cv::String> paths(cvPaths.begin(), cvPaths.end());
    std::sort(paths.begin(), paths.end());

    // 2. Load Images from the paths
    for (const auto& path : paths) {
        cv::Mat img = cv::imread(path);
        if (img.empty()) {
            std::cerr << "Could not load image: " << path << std::endl;
        } else {
            images.push_back(img);
        }
    }

}