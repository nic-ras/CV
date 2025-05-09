#include "ImageLoader.h"
#include <iostream>

int main(int argc, char** argv) {

    // Load images
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <pattern> (e.g. images/*.png)" << std::endl;
        return 1;
    }

    std::string pattern = argv[1];
    std::vector<std::string> imagePaths = getImagePathsFromPattern(pattern);
    if (imagePaths.empty()) {
        std::cerr << "No images found for pattern: " << pattern << std::endl;
        return 1;
    }

    std::vector<cv::Mat> images = loadImages(imagePaths);
    std::cout << "Loaded " << images.size() << " images." << std::endl;

    // Project the images on a cylindrical surface
    std::vector<cv::Mat> projectedImages;


    return 0;
}
