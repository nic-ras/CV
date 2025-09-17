#ifndef LOADER_H
#define LOADER_H

#pragma once

#include <opencv2/core.hpp>
#include <vector>

// Function to load a dataset of images and coordinates
void loadDataset(const cv::String& imageDir,
                 const cv::String& pattern,
                 const cv::String& coordFile,
                 std::vector<cv::Mat>& images,
                 std::vector<std::vector<cv::Point>>& coords);

#endif 