#include "ImageLoader.h"
#include "CylindricalProjection.h"
#include "MatchFeatures.h"
#include "RefineMatches.h"
#include "FindTranslation.h"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>


int main(int argc, char** argv) {

    // Load images
    // (images)
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <directory> <pattern> (e.g. /images i*.png)" << std::endl;
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
    std::cout << "*****" << std::endl;
    std::cout << "Loaded " << images.size() << " images." << std::endl;
    std::cout << "*****" << std::endl;

    // Project the images on a cylindrical surface
    // (projectedImages)
    double angle = 33;
    std::vector<cv::Mat> projectedImages = CylindricalProjection(images, angle);

    // Extract the SIFT features of the images
    // (allKeypoints)
    // (allDescriptors)
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
    
    // Compute the match between the different features
    // (allMatches)
    std::vector<std::vector<cv::DMatch>> allMatches = MatchFeatures (allDescriptors);

    // Refine the matches
    // (refinedMatches)
    double ratio = 2.0;

    std::vector<std::vector<cv::DMatch>> refinedMatches = RefineMatches(allMatches, ratio);

    //  Find the translation between the images
    // (translations)
    std::vector<cv::Point2f> translations = FindTranslation(allKeypoints, refinedMatches, ratio);

    // Compute the final panorama
    // (panorama)
    int totalWidth = projectedImages[0].cols;
    int maxHeight = projectedImages[0].rows;

    for (const auto& t : translations) {
        totalWidth += static_cast<int>(std::abs(t.x)); 
    }

    cv::Mat panorama(maxHeight, totalWidth, projectedImages[0].type(), cv::Scalar::all(0));

    int currentX = 0;

    for (size_t i = 0; i < projectedImages.size(); ++i) {
        const cv::Mat& img = projectedImages[i];
        cv::Mat equalized;

        if (img.channels() == 3) {
            cv::Mat gray;
            cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);          
            cv::equalizeHist(gray, gray);                         
            cv::cvtColor(gray, equalized, cv::COLOR_GRAY2BGR);    
        }
        else if (img.channels() == 1) {
            cv::equalizeHist(img, equalized);
        }
        else {
            equalized = img.clone();
        }

        int maxWidthRemaining = panorama.cols - currentX;
        if (maxWidthRemaining <= 0) {
            std::cerr << "Skipping image " << i << ": no space left in panorama." << std::endl;
            break;
        }
        int roiWidth = std::min(equalized.cols, maxWidthRemaining);
        int roiHeight = std::min(equalized.rows, panorama.rows);

        if (roiWidth > 0 && roiHeight > 0) { 
            cv::Mat roi = panorama(cv::Range(0, roiHeight), cv::Range(currentX, currentX + roiWidth));
            equalized(cv::Rect(0, 0, roiWidth, roiHeight)).copyTo(roi);
        }

        if (i < translations.size()) {
            currentX += static_cast<int>(-translations[i].x);
        }

    }

    // Show the final output
    cv::Mat resized;
    cv::resize(panorama, resized, cv::Size(), 0.5, 0.5); 
    cv::imshow("Panorama", resized);
    cv::waitKey(0);

    return 0;
}
