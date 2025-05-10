#include "ImageLoader.h"
#include "panoramic_utils.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

int main(int argc, char** argv) {

    // Load images
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <pattern> (e.g. images/*.png)" << std::endl;
        return 1;
    }

    cv::String directory = argv[1];                
    cv::String pattern = argv[2]; 
    std::vector<cv::String> imagePaths = getImagePathsFromPattern(directory, pattern);
    if (imagePaths.empty()) {
        std::cerr << "No images found for pattern: " << pattern << std::endl;
        return 1;
    }

    std::vector<cv::Mat> images = loadImages(imagePaths);
    std::cout << "Loaded " << images.size() << " images." << std::endl;

    // Project the images on a cylindrical surface
    std::vector<cv::Mat> projectedImages;
    double angle = 33;

    for (const auto& img : images) {
        cv::Mat projected = cylindricalProj(img, angle);
        projectedImages.push_back(projected);
    }

    std::cout << "Projected " << projectedImages.size() << " images." << std::endl;

    // Extract the SIFT features of the images
    std::vector<std::vector<cv::KeyPoint>> allKeypoints;
    std::vector<cv::Mat> allDescriptors;

    cv::Ptr<cv::SIFT> sift = cv::SIFT::create();

    for (const auto& img : projectedImages) {
        std::vector<cv::KeyPoint> keypoints;
        cv::Mat descriptors;
        sift->detectAndCompute(img, cv::noArray(), keypoints, descriptors);

        allKeypoints.push_back(keypoints);
        allDescriptors.push_back(descriptors);
    }   


    return 0;
}
