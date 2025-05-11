#include "ImageLoader.h"
#include "panoramic_utils.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

int main(int argc, char** argv) {

    // Load images
    // (images)
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
    std::cout << "*****" << std::endl;
    std::cout << "Loaded " << images.size() << " images." << std::endl;
    std::cout << "*****" << std::endl;

    // Project the images on a cylindrical surface
    // (projectedImages)
    std::vector<cv::Mat> projectedImages;
    double angle = 33;

    for (const auto& img : images) {
        cv::Mat projected = cylindricalProj(img, angle);
        projectedImages.push_back(projected);
    }

    std::cout << "Projected " << projectedImages.size() << " images." << std::endl;
    std::cout << "*****" << std::endl;

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
    std::vector<std::vector<cv::DMatch>> allMatches;
    cv::BFMatcher matcher(cv::NORM_L2, true);

    for (size_t i = 0; i < allDescriptors.size() - 1; ++i) {
        const cv::Mat& desc1 = allDescriptors[i];
        const cv::Mat& desc2 = allDescriptors[i + 1];

        std::vector<cv::DMatch> matches;
        matcher.match(desc1, desc2, matches);

        allMatches.push_back(matches);

        std::cout << "Matched image " << i + 1 << " and " << i + 2 << ": " << matches.size() << " matches found." << std::endl;
    }
    std::cout << "*****" << std::endl;

    // Refine the matches
    // (refinedMatches)
    double ratio = 2.0;

    std::vector<std::vector<cv::DMatch>> refinedMatches;

    for (const auto& matches : allMatches) {
        double minDist = std::numeric_limits<double>::max();

        for (const auto& m : matches) {
            if (m.distance < minDist) minDist = m.distance;
        }

        std::vector<cv::DMatch> goodMatches;
        for (const auto& m : matches) {
            if (m.distance < ratio * minDist) {
                goodMatches.push_back(m);
            }
        }

        refinedMatches.push_back(goodMatches);

        std::cout << "Refine match from " << matches.size() << " to " << goodMatches.size() << ". " << std::endl;
    }
    std::cout << "*****" << std::endl;

    //  Find the translation between the images
    // (translations)
    std::vector<cv::Point2f> translations; 

    for (size_t i = 0; i < refinedMatches.size(); ++i) {

        std::vector<cv::Point2f> pts1, pts2;

        for (const auto& match : refinedMatches[i]) {
            pts1.push_back(allKeypoints[i][match.queryIdx].pt);
            pts2.push_back(allKeypoints[i + 1][match.trainIdx].pt);
        }

        cv::Mat H;
        cv::Mat inlierMask;
        if (pts1.size() >= 4 && pts2.size() >= 4) { 
            
            H = cv::findHomography(pts2, pts1, cv::RANSAC, ratio, inlierMask);
        }

        if (!H.empty()) {
            std::vector<cv::Point2f> inlierPts1, inlierPts2;

            for (size_t j = 0; j < inlierMask.rows; ++j) {
                if (inlierMask.at<uchar>(j)) {
                    inlierPts1.push_back(pts1[j]);
                    inlierPts2.push_back(pts2[j]);
                }
            }

            if (inlierPts1.size() >= 4) {
                float dx_sum = 0.0f;
                float dy_sum = 0.0f;

                for (size_t j = 0; j < inlierPts1.size(); ++j) {
                    dx_sum += inlierPts2[j].x - inlierPts1[j].x;
                    dy_sum += inlierPts2[j].y - inlierPts1[j].y;
                }

                float dx = dx_sum / inlierPts1.size();
                float dy = dy_sum / inlierPts1.size();

                translations.push_back(cv::Point2f(dx, dy));

                std::cout << "Translation between image " << i + 1 << " and " << i + 2
                          << ": dx= " << dx << ", dy= " << dy
                          << " (" << inlierPts1.size() << " inliers)" << std::endl;
            }
        }

    }
    std::cout << "*****" << std::endl;

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
