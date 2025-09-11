#include "Loader.h"
#include "Projection.h"
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
        std::cout << "Riga " << i << " coordinate: ";
        for (auto& p : coords[i]) {
            std::cout << "(" << p.x << "," << p.y << ") ";
        }
        std::cout << std::endl;
    }

    // Stampa l'immagine originale e la trasposta 
    std::vector<cv::Mat> results = projectedImages(images, coords);

    for (size_t i = 0; i < results.size(); i++) {
        // ridimensiono l'originale alla stessa dimensione della trasformata
        float scale = 0.2f;
        cv::Mat img = images[i].clone();
        cv::Mat displayImg;
        cv::resize(img, displayImg, cv::Size(), scale, scale);

        if (i >= coords.size()) break;

        for (auto& p : coords[i]) {
            cv::circle(displayImg, cv::Point2f(p.x * scale, p.y * scale), 5, cv::Scalar(0,0,255), -1);
        }

        cv::Mat resizedOriginal;
        cv::resize(displayImg, resizedOriginal, results[i].size());


        // creo immagine affiancata
        cv::Mat sideBySide(resizedOriginal.rows, resizedOriginal.cols * 2, resizedOriginal.type());
        resizedOriginal.copyTo(sideBySide(cv::Rect(0, 0, resizedOriginal.cols, resizedOriginal.rows)));
        results[i].copyTo(sideBySide(cv::Rect(resizedOriginal.cols, 0, results[i].cols, results[i].rows)));

        // Ridimensiono l'immagine affiancata per la finestra (es. 30% della dimensione reale)
        cv::Mat display;
        cv::resize(sideBySide, display, cv::Size(), scale, scale);

        // mostro a video
        std::string winName = "Originale + Trasformata " + std::to_string(i);
        cv::imshow(winName, display);
        cv::waitKey(0);
        cv::destroyWindow(winName);

        // salvo su file (originale dimensione reale)
        std::string outputDir = "../results/";
        cv::utils::fs::createDirectories(outputDir);

        std::string outName = outputDir + "side_by_side_" + std::to_string(i) + ".jpg";
        cv::imwrite(outName, sideBySide);
}

    return 0;
}