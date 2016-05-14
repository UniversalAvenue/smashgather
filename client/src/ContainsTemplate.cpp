#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "GetMSSIM.h"
#include "ContainsTemplate.h"

using namespace std;
using namespace cv;

#include <iostream>
#include <algorithm>

bool largest_area(const vector<Point> a, const vector<Point> b) {
  return
}

void TrimBlackContour(Mat& input, Mat& resized) {

  Mat mask;
  vector<vector<Point>> contours;

  // Create a binary mask of the input image where any pixel that's not black is white
  // Then find the contours of the resulting white rectangle.
  threshold(input, mask, 1, 255, THRESH_BINARY);
  findContours(mask, contours, CV_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

  auto it = minmax_element(contours.begin(), contours.end(), largest_area);

  cout << boundingRect(contours[0]) << endl;
  // cout << *it.first << " " << *it.second << endl;

  if (input.rows > 900) {
    Size size(1600, 900);
    resize(input, resized, size);
  } else {
    Size size(1440, 900);
    resize(input, resized, size);
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
