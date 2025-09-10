#include "Loader.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/core/utils/filesystem.hpp>

int main() {
    std::vector<cv::Mat> images;
    std::vector<std::vector<cv::Point2f>> coords;

    cv::String imgDir = "../dataset/Input/img/";
    cv::String pattern = "*.png"; // legge PNG
    cv::String coordFile = "../dataset/Output/coord/coordinates.txt";

    // Carica immagini e coordinate
    loadDataset(imgDir, pattern, coordFile, images, coords);

    // Stampa la quantità di immagini e righe coordinate
    std::cout << "Numero di immagini caricate: " << images.size() << std::endl;
    std::cout << "Numero di righe di coordinate lette: " << coords.size() << std::endl;

    // MOSTRA L'ORDINE DEI FILE
    std::vector<cv::String> cvPaths;
    cv::utils::fs::glob(imgDir, pattern, cvPaths, false);

    std::cout << "\nOrdine dei file caricati:\n";
    for (size_t i = 0; i < cvPaths.size(); ++i) {
        std::cout << i+1 << ": " << cvPaths[i] << std::endl;
    }

    // Stampa tutte le coordinate lette
    for (size_t i = 0; i < coords.size(); ++i) {
        std::cout << "Riga " << i+1 << " coordinate: ";
        for (auto& p : coords[i]) {
            std::cout << "(" << p.x << "," << p.y << ") ";
        }
        std::cout << std::endl;
    }

    float scale = 0.3f; // ridimensionamento per la visualizzazione

    // Disegna i punti sulle immagini
    for (size_t i = 0; i < images.size(); ++i) {
        cv::Mat img = images[i].clone();
        cv::Mat displayImg;
        cv::resize(img, displayImg, cv::Size(), scale, scale);

        if (i >= coords.size()) break;

        for (auto& p : coords[i]) {
            cv::circle(displayImg, cv::Point2f(p.x * scale, p.y * scale), 5, cv::Scalar(0,0,255), -1);
        }

        std::string winName = "Image " + std::to_string(i+1);
        cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
        cv::imshow(winName, displayImg);
        cv::waitKey(0);
        cv::destroyWindow(winName);
    }

    return 0;
}