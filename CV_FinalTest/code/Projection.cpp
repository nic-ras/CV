#include "Projection.h"
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

std::vector<cv::Mat> projectedImages(
    const std::vector<cv::Mat>& images,
    const std::vector<std::vector<cv::Point2f>>& coords,
    int width, 
    int height 
) {
    std::vector<cv::Mat> warpedImages;

    // Definisco i punti target di un foglio A4
    std::vector<cv::Point2f> dstPoints = {
        {0, 0},
        {static_cast<float>(width - 1), 0},
        {static_cast<float>(width - 1), static_cast<float>(height - 1)},
        {0, static_cast<float>(height - 1)}
    };

    for (size_t i = 0; i < images.size(); i++) {
        if (coords[i].size() != 4) {
            std::cerr << "Errore: immagine " << i << " non ha 4 punti validi!\n";
            continue;
        }

        // Calcolo l'omografia
        cv::Mat H = cv::findHomography(coords[i], dstPoints);

        // Applico la trasformazione prospettica
        cv::Mat warped;
        cv::warpPerspective(images[i], warped, H, cv::Size(width, height));

        warpedImages.push_back(warped);
    }

    return warpedImages;
}