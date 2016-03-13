#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "GetMSSIM.h"
#include "ContainsTemplate.h"

using namespace std;
using namespace cv;

#include <iostream>

bool ContainsTemplate(Mat& input, Mat& templ, double threshold) {
  assert(input.type() == CV_8UC1);
  assert(templ.type() == CV_8UC1);

  // Create the result matrix
  int result_cols = input.cols - templ.cols + 1;
  int result_rows = input.rows - templ.rows + 1;
  Mat result(result_cols, result_rows, CV_32FC1);

  // Match the template against the input image, and normalize resulting matrix
  matchTemplate(input, templ, result, CV_TM_CCOEFF);
  normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

  // Find the best match within the input image and crop that matching area
  Point maxLoc;
  minMaxLoc(result, NULL, NULL, NULL, &maxLoc);
  Rect bounds(maxLoc.x, maxLoc.y, templ.cols, templ.rows);
  Mat cropped = input(bounds);

  // Determine the similarity between the cropped match and the template
  double similarity = GetMSSIM(cropped, templ)[0];

  return similarity > threshold;
}

