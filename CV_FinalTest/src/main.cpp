#include "DetectArea.h"
#include "Loader.h"
#include "Projection.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <opencv2/core/utils/filesystem.hpp>
#include <chrono> 

int main() {
    std::vector<cv::Mat> images;
    std::vector<std::vector<cv::Point>> coords;
    std::vector<std::vector<cv::Point>> FinalCoords;

    cv::String imgDir = "../dataset/Input/img/";
    cv::String pattern = "*.png"; 
    cv::String coordFile = "../dataset/Output/coord/coordinates.txt";

    //Record starting time
    auto start = std::chrono::high_resolution_clock::now();

    // Upload images and coordinates
    loadDataset(imgDir, pattern, coordFile, images, coords);

    std::cout << "##########" << std::endl;

    // Print the amaount of coordinated images and lines
    std::cout << "Number of images uploaded: " << images.size() << std::endl;
    std::cout << "Number of coordinate rows read: " << coords.size() << std::endl;

    std::cout << "##########" << std::endl;

    // Show the order of uploaded files
    std::vector<cv::String> cvPaths;
    cv::utils::fs::glob(imgDir, pattern, cvPaths, false);

    std::cout << "Order of uploaded files: " << std::endl;;
    for (size_t i = 0; i < cvPaths.size(); i++) {
        std::cout << i+1 << ": " << cvPaths[i] << std::endl;
    }

    std::cout << "##########" << std::endl;

    // Print all read coordinates
    for (size_t i = 0; i < coords.size(); i++) {
        std::cout << "Row " << i << " coordinates: ";
        for (auto& p : coords[i]) {
            std::cout << "(" << p.x << "," << p.y << ") ";
        }
        std::cout << std::endl;
    }

    std::cout << "##########" << std::endl;

    // Identification of the corners of the document
    for (size_t i = 0; i < images.size(); i++) {
        std::vector<cv::Point> corners;
        std::cout << "Working on image: "<< i << std::endl;
        int res = DetectArea(images[i], &corners);
        
        if (res == 0 && corners.size() == 4) {
            FinalCoords.push_back(corners);
        } else {
            std::cerr << "Error: 4 corners not found per image " << i << std::endl;
        } 
    }

    std::cout << "##########" << std::endl;
    
    // Print the original image and the identified document
    std::vector<cv::Mat> results = projectedImages(images, FinalCoords); 

    // Enhancement of the detected image
    std::vector<cv::Mat> doc;
    for (size_t i = 0; i < results.size(); i++) {
        cv::Mat gray, sharpened;

        // 1) Grayscale
        cv::cvtColor(results[i], gray, cv::COLOR_BGR2GRAY);

        // 2) Sharpening 
        cv::Mat kernel = (cv::Mat_<float>(3,3) <<
            0, -1,  0,
            -1,  5, -1,
            0, -1,  0);
        cv::filter2D(gray, sharpened, -1, kernel);

        doc.push_back(sharpened);
    }

    auto end = std::chrono::high_resolution_clock::now();

    // Show the results
    for (size_t i = 0; i < doc.size(); i++) {

        // Resize the original to the same size as the transformed image
        // ALTRIMENTI UN RESIZE CON DIMENSIONI FISSE E POI FINESTRA MODIFICABILE COME FA RICCARDO
        float scale = 0.2f;
        cv::Mat img = images[i].clone();
        cv::Mat resizedOriginal;
        cv::resize(img, resizedOriginal, doc[i].size());

        double scaleX = static_cast<double>(resizedOriginal.cols) / images[i].cols;
        double scaleY = static_cast<double>(resizedOriginal.rows) / images[i].rows;

        for (auto& p : FinalCoords[i]) {
            cv::Point2f scaledPt(p.x * scaleX, p.y * scaleY);
            cv::circle(resizedOriginal, scaledPt, 15, cv::Scalar(0,0,0), -1);
            cv::circle(resizedOriginal, scaledPt, 10, cv::Scalar(0,0,255), -1);
        }

        cv::Mat docColor;
        cv::cvtColor(doc[i], docColor, cv::COLOR_GRAY2BGR);

        // Side by side image
        cv::Mat sideBySide;
        cv::hconcat(resizedOriginal, docColor, sideBySide);

        cv::Mat display;
        cv::resize(sideBySide, display, cv::Size(), scale, scale);

        // Show final output
        std::string winName = "Original + Output " + std::to_string(i);
        cv::namedWindow(winName, WINDOW_NORMAL);
        cv::imshow(winName, display);
        cv::waitKey(0);
        cv::destroyWindow(winName);

        // Save the files
        std::string outputDir = "../results/";
        cv::utils::fs::createDirectories(outputDir);

        std::string outName = outputDir + "side_by_side_" + std::to_string(i) + ".png";
        cv::imwrite(outName, sideBySide);
    }

    std::cout << "##########" << std::endl;

    //Distances point-to-point
    std::vector<std::vector<double>> distances(coords.size());
    distances.push_back({});    
    std::cout << "Distance between the predicted and ground truth corners of images " << std::endl;
    for(size_t i=0; i<coords.size(); i++)
    {
        std::cout << i << ": ";
        for(size_t j=0; j<coords[i].size(); j++)
        {
            cv::Point pt1 = FinalCoords[i][j]; 
            cv::Point pt2 = coords[i][j];
            double dst = norm(pt1-pt2);
            distances[i].push_back(dst);
            std::cout << dst << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "##########" << std::endl;

    //Area difference
    std::vector<cv::Vec2d> area_differences(coords.size());
    area_differences.clear();
    area_differences.resize(coords.size());       //Allocating space
    std::cout << "Difference of the predicted document area and the ground truth " << std::endl;
    std::cout << "(Absolute area difference) ; (Relative area difference)" << std::endl;
    for(size_t i=0; i<coords.size(); i++)
    {
        double area1 = contourArea(coords[i]);
        double area2 = contourArea(FinalCoords[i]);
        double total_area = images[i].rows*images[i].cols;
        area_differences[i][0] = std::fabs(area1-area2);    //Absolute area difference
        area_differences[i][1] = std::fabs(area1-area2)/total_area; //Relative area difference
        std::cout << i << ": "
        << area_differences[i][0] << " ; " << area_differences[i][1] * 100 << "% " << std::endl;
    }

    std::cout << "##########" << std::endl;

    //Computational time

    //Compute duration in milliseconds
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Execution time: " << elapsed.count() << " ms" << std::endl;

    std::cout << "##########" << std::endl;

    return 0;
}