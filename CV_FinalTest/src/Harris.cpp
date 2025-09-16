#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace cv;
using namespace std;

// Funzione per trovare intersezione di due linee in formato (rho, theta)
bool intersection(const Vec2f &line1, const Vec2f &line2, Point2f &pt, Size imgSize) {
    double rho1 = line1[0], theta1 = line1[1];
    double rho2 = line2[0], theta2 = line2[1];

    double det = cos(theta1) * sin(theta2) - sin(theta1) * cos(theta2);
    if (fabs(det) < 1e-10) return false; // linee parallele

    double x = (sin(theta2) * rho1 - sin(theta1) * rho2) / det;
    double y = (-cos(theta2) * rho1 + cos(theta1) * rho2) / det;

    if (x < 0 || x >= imgSize.width || y < 0 || y >= imgSize.height)
        return false;

    pt = Point2f((float)x, (float)y);
    return true;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <immagine>" << endl;
        return -1;
    }

    string imagePath = argv[1];
    Mat image = imread(imagePath);
    if (image.empty()) {
        cout << "Errore caricamento immagine!" << endl;
        return -1;
    }

    Mat original = image.clone();

    // Ridimensionamento iniziale
    Mat resized;
    resize(image, resized, Size(480, 720)); 

    // Grayscale
    Mat grayscale, equalized, blurred;
    cvtColor(resized, grayscale, COLOR_BGR2GRAY);
    GaussianBlur(grayscale, blurred, Size(5,5), 0);
    imshow("grayscale", blurred);
    waitKey(0);

    Mat sharpened, mask, grad_x, grad_y, abs_grad_x, abs_grad_y, grad;

    /*
    // Sharpening con Laplacian
    Mat blurred, lap, lap_abs, sharpened;
    Laplacian(grayscale, lap, CV_16S, 5);
    convertScaleAbs(lap, lap_abs);
    addWeighted(grayscale, 1, lap_abs, 1, 0, sharpened);
    imshow("sharpened", lap);
    waitKey(0);
    */

    //equalizeHist(grayscale, equalized);

    //imshow("equalized", equalized);
    //waitKey(0);

    //Sobel(grayscale, grad_x, CV_16S, 1, 0, 1);
    //Sobel(grayscale, grad_y, CV_16S, 0, 1, 1);

    // Conversione in 8-bit
    //convertScaleAbs(grad_x, abs_grad_x);
    //convertScaleAbs(grad_y, abs_grad_y);

    // Combinazione dei gradienti
    //addWeighted(abs_grad_x, 0.25, abs_grad_y, 0.25, 0, blurred);

    //imshow("Sobel Edges", blurred);
    //waitKey(0);

    // Sfocatura con Gaussiana
    //GaussianBlur(equalized, blurred, Size(5,5), 1.0);

    // Differenza (dettagli ad alta frequenza)
    //subtract(equalized, blurred, mask);

    // Aggiungi i dettagli all’originale
    //addWeighted(equalized, 1.0, mask, 1.5, 0, sharpened);

    // Threshold
    /*
    Mat binary;
    threshold(equalized, binary, 160, 255, THRESH_BINARY);
    imshow("binary", binary);
    waitKey(0);

    */
    // Morphological filters
    /*
    Mat morph;
    Mat kernel1 = getStructuringElement(MORPH_RECT, Size(2,2));
    Mat kernel2 = getStructuringElement(MORPH_RECT, Size(5,5));
    cv::erode(binary, binary, kernel1);
    GaussianBlur(binary, binary, Size(5,5), 0);
    //morphologyEx(binary, morph, MORPH_OPEN, kernel1);
    //morphologyEx(morph, morph, MORPH_CLOSE, kernel2);
    imshow("morphology", binary);
    waitKey(0);
    */

    // Canny
    Mat canny;
    Canny(blurred, canny, 80, 200);
    imshow("Canny", canny);
    waitKey(0);

    // Hough Transform
    vector<Vec2f> lines;
    HoughLines(canny, lines, 1, CV_PI/180, 90); // rho, theta, soglia

    // Disegna linee trovate (debug)
    Mat lineImg = resized.clone();
    for (size_t i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(lineImg, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
    }
    imshow("Hough Lines", lineImg);
    waitKey(0);

    // Calcola intersezioni
    vector<Point2f> corners;
    for (size_t i = 0; i < lines.size(); i++) {
        for (size_t j = i+1; j < lines.size(); j++) {
            Point2f pt;
            if (intersection(lines[i], lines[j], pt, resized.size())) {
                corners.push_back(pt);
            }
        }
    }

    if (corners.size() < 4) {
        cout << "Documento non rilevato." << endl;
        waitKey(0);
        return -1;
    }

    // Prendi i 4 punti estremi (bounding box convesso)
    RotatedRect box = minAreaRect(corners);
    Point2f rectPts[4];
    box.points(rectPts);

    for (int i = 0; i < 4; i++) {
        line(resized, rectPts[i], rectPts[(i+1)%4], Scalar(0,255,0), 3);
        circle(resized, rectPts[i], 8, Scalar(255,0,0), -1);
    }

    imshow("Detected Document", resized);
    waitKey(0);
    destroyAllWindows();

    return 0;
}