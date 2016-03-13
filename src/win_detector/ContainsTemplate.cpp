#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "GetMSSIM.h"
#include "ContainsTemplate.h"

using namespace std;
using namespace cv;

bool ContainsTemplate(Mat& input, Mat& templ, double threshold) {
  // Convert to grayscale
  Mat gray_input, gray_templ;
  cvtColor(input, gray_input, COLOR_BGR2GRAY);
  cvtColor(templ, gray_templ, COLOR_BGR2GRAY);

  // Create the result matrix
  int result_cols = input.cols - templ.cols + 1;
  int result_rows = input.rows - templ.rows + 1;
  Mat result(result_cols, result_rows, CV_32FC1);

  // Do the Matching and Normalize
  matchTemplate(input, templ, result, CV_TM_CCOEFF);
  normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

  Point maxLoc;
  minMaxLoc(result, NULL, NULL, NULL, &maxLoc);
  Rect bounds(maxLoc.x, maxLoc.y, templ.cols, templ.rows);
  Mat cropped = input(bounds);
  double similarity = GetMSSIM(cropped, templ)[0];

  return similarity > threshold;
}

