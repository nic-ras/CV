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
    cv::String pattern = "*.png"; 
    cv::String coordFile = "../dataset/Output/coord/coordinates.txt";

    // Upload images and coordinates
    loadDataset(imgDir, pattern, coordFile, images, coords);

    // Print the amount of coordinated images and lines
    std::cout << "Number of images uploaded: " << images.size() << std::endl;
    std::cout << "Number of coordinate rows read: " << coords.size() << std::endl;

    // Show the order of uploaded files
    std::vector<cv::String> cvPaths;
    cv::utils::fs::glob(imgDir, pattern, cvPaths, false);

    std::cout << "Order of uploaded files: " << std::endl;;
    for (size_t i = 0; i < cvPaths.size(); ++i) {
        std::cout << i+1 << ": " << cvPaths[i] << std::endl;
    }

    // Print all read coordinates
    for (size_t i = 0; i < coords.size(); ++i) {
        std::cout << "Row " << i << " coordinates: ";
        for (auto& p : coords[i]) {
            std::cout << "(" << p.x << "," << p.y << ") ";
        }
        std::cout << std::endl;
    }

    // Identification of the vertices of the document


    // Print the original image and the identified document
    std::vector<cv::Mat> results = projectedImages(images, coords); // SOSTITUIRE COORDS CON I VERTICI TROVATI

    // Enhancement of the detected image
    std::vector<cv::Mat> doc;
    for (size_t i = 0; i < results.size(); i++) {
    cv::Mat gray, equalized, sharpened;

    // 1) Grayscale
    cv::cvtColor(results[i], gray, cv::COLOR_BGR2GRAY);

    // 2) Equalize the Histogram
    cv::equalizeHist(gray, equalized);

    // 3) Sharpen 
    cv::Mat kernel = (cv::Mat_<float>(3,3) <<
        0, -1,  0,
       -1,  5, -1,
        0, -1,  0);
    cv::filter2D(equalized, sharpened, -1, kernel);

    doc.push_back(sharpened);
    }

    for (size_t i = 0; i < doc.size(); i++) {

        // Resize the original to the same size as the transformed image
        // ALTRIMENTI UN RESIZE CON DIMENSIONI FISSE E POI FINESTRA MODIFICABILE COME FA RICCARDO
        float scale = 0.2f;
        cv::Mat img = images[i].clone();
        cv::Mat displayImg;
        cv::resize(img, displayImg, cv::Size(), scale, scale);

        if (i >= coords.size()) break;

        for (auto& p : coords[i]) {
            cv::circle(displayImg, cv::Point2f(p.x * scale, p.y * scale), 5, cv::Scalar(0,0,255), -1);
        }

        cv::Mat resizedOriginal;
        cv::resize(displayImg, resizedOriginal, doc[i].size());

        // Side by side image
        cv::Mat sideBySide(resizedOriginal.rows, resizedOriginal.cols * 2, resizedOriginal.type());
        resizedOriginal.copyTo(sideBySide(cv::Rect(0, 0, resizedOriginal.cols, resizedOriginal.rows)));
        results[i].copyTo(sideBySide(cv::Rect(resizedOriginal.cols, 0, doc[i].cols, doc[i].rows)));

        cv::Mat display;
        cv::resize(sideBySide, display, cv::Size(), scale, scale);

        // Show final output
        std::string winName = "Original + Output " + std::to_string(i);
        cv::imshow(winName, display);
        cv::waitKey(0);
        cv::destroyWindow(winName);

        // Save the files
        std::string outputDir = "../results/";
        cv::utils::fs::createDirectories(outputDir);

        std::string outName = outputDir + "side_by_side_" + std::to_string(i) + ".png";
        cv::imwrite(outName, sideBySide);
    }

    return 0;
}