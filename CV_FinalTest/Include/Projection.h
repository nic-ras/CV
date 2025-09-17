#ifndef PROJECTION_H
#define PROJECTION_H

#pragma once

#include <opencv2/core.hpp>
#include <vector>
#include <iostream>

// Function that projects images onto an A4 rectangle
std::vector<cv::Mat> projectedImages(
    const std::vector<cv::Mat>& images,
    const std::vector<std::vector<cv::Point>>& coords,
    int width = 2000,   // A4 pixel for 300 dpi
    int height = 2650   // A4 pixel for 300 dpi
);

#endif 