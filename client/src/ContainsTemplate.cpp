#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "GetMSSIM.h"
#include "ContainsTemplate.h"

using namespace std;
using namespace cv;

#include <iostream>
#include <algorithm>

// Find the actual image coords ignoring any black borders
Rect FindImageRect(Mat &mask) {

  if (mask.type() != CV_8UC1) {
    Mat grayscale;
    cvtColor(mask, grayscale, CV_BGR2GRAY);
    return FindImageRect(grayscale);
  }

  Point a, b;

  for (size_t i = 0; i < mask.rows; i++) {
    auto row = mask.row(i);

    // Check if row contains any non-black pixels
    auto is_black = std::all_of(row.begin<uint8_t>(), row.end<uint8_t>(), [](uint8_t pixel){ return pixel == 0; });

    if (!is_black) {
      a.y = i;
      break;
    }
  }

  for (size_t i = mask.rows-1; i > 0; i--) {
    auto row = mask.row(i);

    // Check if row contains any non-black pixels
    auto is_black = std::all_of(row.begin<uint8_t>(), row.end<uint8_t>(), [](uint8_t pixel){ return pixel == 0; });

    if (!is_black) {
      b.y = i + 1;
      break;
    }
  }

  for (size_t i = 0; i < mask.cols; i++) {
    auto col = mask.col(i);

    // Check if col contains any non-black pixels
    auto is_black = std::all_of(col.begin<uint8_t>(), col.end<uint8_t>(), [](uint8_t pixel){ return pixel == 0; });

    if (!is_black) {
      a.x = i;
      break;
    }
  }

  for (size_t i = mask.cols-1; i != 0; i--) {
    auto col = mask.col(i);

    // Check if col contains any non-black pixels
    auto is_black = std::all_of(col.begin<uint8_t>(), col.end<uint8_t>(), [](uint8_t pixel){ return pixel == 0; });

    if (!is_black) {
      b.x = i + 1;
      break;
    }
  }

  return Rect(a, b);
}

void TrimBlackContour(Mat& input, Mat& resized) {

  auto roi = FindImageRect(input);

  // Our templates are scaled for the 826px high screenshots, so if the screen is
  // a different size, we have to scale it to the 826px height.
  //
  // Aspect ratio of the image should be 1.36
  if (roi.height == 826) {
    input(roi).copyTo(resized);
  } else {
    Size size(roi.width * 826 / roi.height, 826);
    resize(input(roi), resized, size);
  }
}

bool ContainsTemplate(Mat& input, Mat& templ, double threshold) {
  assert(input.type() == CV_8UC1);
  assert(templ.type() == CV_8UC1);

  // Trim the black contour and resize the input
  Mat resized;
  TrimBlackContour(input, resized);

  // Create the result matrix
  int result_cols = resized.cols - templ.cols + 1;
  int result_rows = resized.rows - templ.rows + 1;
  Mat result(result_cols, result_rows, CV_32FC1);

  // Match the template against the resized image, and normalize resulting matrix
  matchTemplate(resized, templ, result, CV_TM_CCOEFF);
  normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

  // Find the best match within the resized image and crop that matching area
  Point maxLoc;
  minMaxLoc(result, NULL, NULL, NULL, &maxLoc);
  Rect bounds(maxLoc.x, maxLoc.y, templ.cols, templ.rows);
  Mat cropped = resized(bounds);

  // Determine the similarity between the cropped match and the template
  double similarity = GetMSSIM(cropped, templ)[0];

  return similarity > threshold;
}
