#ifndef PROJECTION_H
#define PROJECTION_H

#include <opencv2/core.hpp>
#include <vector>
#include <iostream>

// Funzione che proietta le immagini in un rettangolo A4
std::vector<cv::Mat> projectedImages(
    const std::vector<cv::Mat>& images,
    const std::vector<std::vector<cv::Point2f>>& coords,
    int width = 2000,   // larghezza A4 a 300 dpi
    int height = 2650   // altezza A4 a 300 dpi
);

#endif // PROJECTION_H