# Automatic Document Scanner

This project simulates the behavior of a real scanner by processing an image of a physical document and producing a top-down, binarized, and perspective-corrected version.

## Project Steps

### 1. Load Input Data (Nicolò)
- Load the input images (photos of documents).
- Load the ground truth output images.
- Load the `coordinates.txt` file, which contains the correct corner positions of each document.

### 2. Detect Document Area (Riccardo)
- Detect the four corners of the document in each input image.
- Identify and extract the region of interest (ROI) corresponding to the document.

### 3. Process the Document Image

#### 3.1 Geometric Transformation (Nicolò)
- Compute a planar perspective transformation matrix.
- Warp the image so that the document is displayed with proportions corresponding to an A4 sheet (210 mm × 297 mm).
  
#### 3.2 Enhance Visibility (Riccardo) 
- Improve the readability of the document by applying techniques such as:
  - Contrast enhancement
  - Binarization (e.g., Otsu or adaptive thresholding)
  - Denoising filters

### 4. Evaluate Performance
- **Corner Detection Accuracy**: Measure the Euclidean distance between the predicted and ground truth corners. 
- **IoU (Intersection over Union)**: Compare the predicted document area with the ground truth area.
- **Processing Time**: Record the average execution time per image.

## File Structure
project/

├── input/                # Input document images

├── output/               # Processed and transformed document images

├── coordinates.txt       # Ground truth corner positions

├── src/                  # Source code for detection, processing, and evaluation

├── README.md             # Project documentation

└── requirements.txt      # Dependencies list
