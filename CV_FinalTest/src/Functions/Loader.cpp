/************************************************************
 *  Project : Document Scanner 
 *  File    : Loader.cpp
 *  Author  : Nicolò Rasera
 *  Course  : Computer Vision 
 *  Teacher : Stefano Ghidoni
 ************************************************************/
#include "Loader.h"
#include <opencv2/core.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/imgcodecs.hpp>
#include <fstream>
#include <iostream>

void loadDataset(const cv::String& imageDir,
                 const cv::String& pattern,
                 const cv::String& coordFile,
                 std::vector<cv::Mat>& images,
                 std::vector<std::vector<cv::Point>>& coords)
{
    // 1. Get the image paths
    std::vector<cv::String> cvPaths;
    cv::utils::fs::glob(imageDir, pattern, cvPaths, false);

    std::vector<cv::String> paths(cvPaths.begin(), cvPaths.end());

    // 2. Load Images from the paths
    for (const auto& path : paths) {
        cv::Mat img = cv::imread(path);
        if (img.empty()) {
            std::cerr << "Could not load image: " << path << std::endl;
        } else {
            images.push_back(img);
        }
    }

    // 3. Load coordinates from the txt file
    std::ifstream infile(coordFile);
    if (!infile.is_open()) {
        std::cerr << "Could not open file: " << coordFile << std::endl;
        return;
    }

    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;

        size_t pos = line.find(':');
        if (pos == std::string::npos) continue;

        std::string rest = line.substr(pos + 1);
        std::vector<cv::Point> rowPoints;
        size_t start = 0;

        while ((start = rest.find('"', start)) != std::string::npos) {
            size_t end = rest.find('"', start + 1);
            if (end == std::string::npos) break;

            std::string pair = rest.substr(start + 1, end - start - 1); 
            std::stringstream ss(pair);
            float x, y;
            ss >> x >> y;
            rowPoints.emplace_back(x, y);

            start = end + 1;
        }

        if (!rowPoints.empty())
            coords.push_back(rowPoints);
        else
            std::cerr << "Errore parsing riga: " << line << std::endl;
    }

}