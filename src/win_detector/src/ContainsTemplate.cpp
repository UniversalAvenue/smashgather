#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "GetMSSIM.h"
#include "ContainsTemplate.h"

using namespace std;
using namespace cv;

bool ContainsTemplate(Mat& input, Mat& templ, double threshold) {
  // Convert to grayscale for performance
  Mat gray_input, gray_templ;
  cvtColor(input, gray_input, COLOR_BGR2GRAY);
  cvtColor(templ, gray_templ, COLOR_BGR2GRAY);

  // Create the result matrix
  int result_cols = gray_input.cols - gray_templ.cols + 1;
  int result_rows = gray_input.rows - gray_templ.rows + 1;
  Mat result(result_cols, result_rows, CV_32FC1);

  // Match the template against the input image, and normalize resulting matrix
  matchTemplate(gray_input, gray_templ, result, CV_TM_CCOEFF);
  normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

  // Find the best match within the input image and crop that matching area
  Point maxLoc;
  minMaxLoc(result, NULL, NULL, NULL, &maxLoc);
  Rect bounds(maxLoc.x, maxLoc.y, gray_templ.cols, gray_templ.rows);
  Mat cropped = gray_input(bounds);

  // Determine the similarity between the cropped match and the template
  double similarity = GetMSSIM(cropped, gray_templ)[0];

  return similarity > threshold;
}

