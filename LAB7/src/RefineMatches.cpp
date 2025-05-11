#include <opencv2/features2d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>

std::vector<std::vector<cv::DMatch>> RefineMatches(std::vector<std::vector<cv::DMatch>> allMatches,  double ratio){
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

    return refinedMatches;

}