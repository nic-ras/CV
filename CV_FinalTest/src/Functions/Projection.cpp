/************************************************************
 *  Project : Document Scanner 
 *  File    : Projection.cpp
 *  Author  : Nicolò Rasera
 *  Course  : Computer Vision 
 *  Teacher : Stefano Ghidoni
 ************************************************************/
#include "Projection.h"
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

std::vector<cv::Mat> projectedImages(
    const std::vector<cv::Mat>& images,
    const std::vector<std::vector<cv::Point>>& coords,
    int width, 
    int height 
) {
    std::vector<cv::Mat> warpedImages;

    // A4 sheet
    std::vector<cv::Point2f> dstPoints = {
        {0, 0},
        {static_cast<float>(width - 1), 0},
        {static_cast<float>(width - 1), static_cast<float>(height - 1)},
        {0, static_cast<float>(height - 1)}
    };

    for (size_t i = 0; i < images.size(); i++) {
        if (coords[i].size() != 4) {
            std::cerr << "Error: image " << i << " he doesn't have 4 valid points!" << std::endl;
            continue;
        }

        // Calculate the homography
        cv::Mat H = cv::findHomography(coords[i], dstPoints);

        // Apply the perspective transformation
        cv::Mat warped;
        cv::warpPerspective(images[i], warped, H, cv::Size(width, height));

        warpedImages.push_back(warped);
    }

    return warpedImages;
}