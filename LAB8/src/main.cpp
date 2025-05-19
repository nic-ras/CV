#include "ImageLoader.h"
#include "create3DChessboardCorners.h"

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>


int main(int argc, char** argv) {

    // Load images
    // (images)
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <directory> <pattern> (e.g. /images i*.png)" << std::endl;
        return 1;
    }

    cv::String directory = argv[1];                
    cv::String pattern = argv[2]; 
    std::vector<cv::String> imagePaths = getImagePathsFromPattern(directory, pattern);
    if (imagePaths.empty()) {
        std::cerr << "No images found for pattern: " << pattern << std::endl;
        return 1;
    }

    std::vector<cv::Mat> images = loadImages(imagePaths);
    std::cout << "*****" << std::endl;
    std::cout << "Loaded " << images.size() << " images." << std::endl;
    std::cout << "*****" << std::endl;

    // Detects the checkerboard
    // 
    std::vector<std::vector<cv::Point2f>> imagePoints;
    std::vector<std::vector<cv::Point3f>> objectPoints;
    float squareSize = 0.11; // 11 cm

    for (const auto& img : images) {

        cv::Size patternsize(6,5); 
        cv::Mat gray;
        cv::cvtColor(img,gray,cv::COLOR_BGR2GRAY);
        std::vector<cv::Point2f> corners;
        
        bool patternfound = cv::findChessboardCorners(gray, patternsize, corners,
                        cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

        if (patternfound) {
            cv::cornerSubPix(gray, corners, cv::Size(11,11), cv::Size(-1,-1),
            cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));

            cv::drawChessboardCorners(img, patternsize, cv::Mat(corners), patternfound);
            cv::imshow("Chessboard detection", img);
            cv::waitKey(100);

            imagePoints.push_back(corners);
            objectPoints.push_back(create3DChessboardCorners(patternsize, squareSize));
        }   
        
    } 
    cv::destroyAllWindows();
    

    // Calibrates the camera
    // 
    cv::Mat cameraMatrix, distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;

    if (imagePoints.size() < 1) {
        std::cerr << "Not enough valid checkerboard detections for calibration." << std::endl;
        std::cout << "*****" << std::endl;
        return 1;
    }

    double rms = cv::calibrateCamera(objectPoints, imagePoints, images[0].size(), cameraMatrix, distCoeffs, rvecs, tvecs);
    std::cout << "Camera calibration RMS error: " << rms << std::endl;
    std::cout << "*****" << std::endl;
    std::cout << "Camera Matrix:\n" << cameraMatrix << std::endl;
    std::cout << "*****" << std::endl;
    std::cout << "Distortion Coefficients:\n" << distCoeffs << std::endl;
    std::cout << "*****" << std::endl;
    

    // Mean reprojection error
    //
    double totalError = 0;
    int totalPoints = 0;

    for (size_t i = 0; i < objectPoints.size(); ++i) {
        std::vector<cv::Point2f> projectedPoints;
        cv::projectPoints(objectPoints[i], rvecs[i], tvecs[i], cameraMatrix, distCoeffs, projectedPoints);

        double err = cv::norm(imagePoints[i], projectedPoints, cv::NORM_L2);
        int n = (int)objectPoints[i].size();
        totalError += err * err;
        totalPoints += n;
    }

    double meanError = std::sqrt(totalError / totalPoints);
    std::cout << "Mean Reprojection Error: " << meanError << std::endl;
    std::cout << "*****" << std::endl;
    

    // Undistorts and rectifies a new image
    //
    std::string newImagePath = "../Images/data/test_image.png"; 
    cv::Mat newImage = cv::imread(newImagePath);

    if (newImage.empty()) {
        std::cerr << "Failed to load new image from " << newImagePath << std::endl;
        return 1;
    }

    cv::Mat undistorted, map1, map2;
    cv::Size imageSize = newImage.size();
    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
                                 cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 0, imageSize, 0),
                                 imageSize, CV_16SC2, map1, map2);

    cv::remap(newImage, undistorted, map1, map2, cv::INTER_LINEAR);

    //  Shows the distorted and undistorted images
    // 
    cv::Mat combined;
    int width = newImage.cols;
    int height = newImage.rows;

    cv::hconcat(newImage, undistorted, combined); 
    cv::Mat resized;
    cv::resize(combined, resized, cv::Size(), 0.2, 0.2); 
    cv::imshow("Distorted (Left) vs Undistorted (Right)", resized);
    cv::waitKey(0);
    cv::destroyAllWindows();
    
    return 0;
}
