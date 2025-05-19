#ifndef CREATE_3D_H
#define CREATE_3D_H

#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

std::vector<cv::Point3f> create3DChessboardCorners(cv::Size boardSize, float squareSize);

#endif // IMAGE_LOADER_H