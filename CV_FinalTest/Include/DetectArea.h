/************************************************************
 *  Project : Document Scanner 
 *  File    : DetectArea.h
 *  Author  : Riccardo Grosso
 *  Course  : Computer Vision 
 *  Teacher : Stefano Ghidoni
 ************************************************************/
#ifndef DOCUMENT_DETECTOR_H
#define DOCUMENT_DETECTOR_H

#pragma once

#include <vector>
#include <opencv2/core.hpp> 

typedef struct
{
    int lines;
    int points;
    std::vector<cv::Point> point_vec;
} result;

// Draw Hough lines on a copy of the provided image.
cv::Mat drawHoughLines(const cv::Mat& image, std::vector<cv::Vec3f> lines, cv::Scalar color);

// Calculates the intersection of two lines expressed in Hough coordinates
cv::Point findIntersection(cv::Vec3f line1, cv::Vec3f line2);

// Select a point (corner) for each of the 4 past point clusters
int findCorners(std::vector<std::vector<cv::Point>> clustered_points, std::vector<cv::Point>* corners);

// Detects the area (the 4 corners) of a document within an image
int DetectArea(const cv::Mat& image, std::vector<cv::Point>* coordinates);

#endif 