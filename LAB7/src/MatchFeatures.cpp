#include <opencv2/features2d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

std::vector<std::vector<cv::DMatch>> MatchFeatures(std::vector<cv::Mat> allDescriptors){
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

    return allMatches;
}