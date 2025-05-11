#include "CylindricalProjection.h"
#include "panoramic_utils.h"
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

std::vector<cv::Mat> CylindricalProjection(std::vector<cv::Mat> images, double angle){
    std::vector<cv::Mat> projectedImages;

    for (const auto& img : images) {
        cv::Mat projected = cylindricalProj(img, angle);
        projectedImages.push_back(projected);
    }

    std::cout << "Projected " << projectedImages.size() << " images." << std::endl;
    std::cout << "*****" << std::endl;

    return projectedImages;

}