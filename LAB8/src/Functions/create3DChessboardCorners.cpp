#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

std::vector<cv::Point3f> create3DChessboardCorners(cv::Size boardSize, float squareSize) {
    std::vector<cv::Point3f> corners;
    for (int i = 0; i < boardSize.height; ++i){
        for (int j = 0; j < boardSize.width; ++j){
            corners.emplace_back(j * squareSize, i * squareSize, 0);
        }
    }
    return corners;
}