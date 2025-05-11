#include <opencv2/features2d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

std::vector<cv::Point2f> FindTranslation(std::vector<std::vector<cv::KeyPoint>> allKeypoints, 
    std::vector<std::vector<cv::DMatch>> refinedMatches, 
    double ratio)
    {
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

    return translations;

}