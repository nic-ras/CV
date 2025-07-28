#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

struct CallbackData {
    cv::Mat* image;
    std::vector<cv::Point>* points;
    int click_count = 0;
};

void onMouseClick(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        CallbackData* data = static_cast<CallbackData*>(userdata);
        if (data->click_count < 4) {
            data->points->emplace_back(x, y);
            std::cout << "Click " << data->click_count + 1 << ": (" << x << ", " << y << ")" << std::endl;
            data->click_count++;

            // Draw a small circle on the image for feedback
            cv::circle(*data->image, cv::Point(x, y), 4, cv::Scalar(0, 0, 255), cv::FILLED);
            cv::imshow("Image", *data->image);
        }
    }
}

int main() {
    std::string folder_path = "./Input";
    std::ofstream outfile("coordinates.txt");

    if (!outfile.is_open()) {
        std::cerr << "Could not open coordinates.txt for writing.\n";
        return 1;
    }

    for (const auto& entry : fs::directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            std::string image_path = entry.path().string();
            cv::Mat img = cv::imread(image_path);

            if (img.empty()) {
                std::cerr << "Could not open image: " << image_path << std::endl;
                continue;
            }

            std::vector<cv::Point> clicked_points;
            CallbackData data{ &img, &clicked_points };

            cv::namedWindow("Image", cv::WINDOW_NORMAL);
            cv::resizeWindow("Image", 800, 600);
            cv::imshow("Image", img);
            cv::setMouseCallback("Image", onMouseClick, &data);

            std::cout << "Click on 4 points for image: " << image_path << std::endl;
            while (data.click_count < 4) {
                int key = cv::waitKey(20);
                if (key == 27) { // ESC to exit early
                    outfile.close();
                    return 0;
                }
            }

            // Write coordinates to file, one per line
            for (const auto& pt : clicked_points) {
                outfile << pt.x << " " << pt.y << std::endl;
            }
        }
    }

    outfile.close();
    std::cout << "All coordinates saved to coordinates.txt" << std::endl;
    return 0;
}