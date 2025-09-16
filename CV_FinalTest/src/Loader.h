#ifndef LOADER_H
#define LOADER_H

#include <opencv2/core.hpp>
#include <vector>

// Funzione per caricare un dataset di immagini e coordinate
// imageDir  : cartella contenente le immagini
// pattern   : pattern dei file immagine (es. "*.jpg", "*.png")
// coordFile : file di testo con le coordinate
// images    : vettore di immagini caricate
// coords    : vettore di vettori di punti (4 punti per ogni immagine)
void loadDataset(const cv::String& imageDir,
                 const cv::String& pattern,
                 const cv::String& coordFile,
                 std::vector<cv::Mat>& images,
                 std::vector<std::vector<cv::Point2f>>& coords);

#endif 