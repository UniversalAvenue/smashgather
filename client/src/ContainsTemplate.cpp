#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/text.hpp>

#include "GetMSSIM.h"
#include "ContainsTemplate.h"

using namespace std;
using namespace cv;
using namespace cv::text;

#include <iostream>
#include <algorithm>
#include <exception>

// Exception thrown when processing an image that's not a valid game screenshot
class InvalidScreenshot: public exception {
  const char* what() const throw() {
    return "The input image is not a valid screenshot";
  }
};

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

  // Aspect ratio of the image should be 1.36 or 1.37 (with glitches). If it's not, throw an error.
  int ratio = round((double) roi.width / roi.height * 100);
  cout << ratio << endl;
  // Except if the ratio is 134. It's likely a screenshoot containg the emulator toolbar.
  if (ratio == 133) {
    roi.height = roi.width / 1.3636363636; // Trim the bottom
  } else if(!(ratio == 136 || ratio == 137)) {
    throw InvalidScreenshot();
  }

  // Our templates are scaled for the 826px high screenshots, so if the screen is
  // a different size, we have to scale it to the 826px height.
  cout << roi.height << endl;
  if (roi.height == 300) {
    input(roi).copyTo(resized);
  } else {
    Size size(roi.width * 826 / roi.height, 826);
    resize(input(roi), resized, size);
  }
}

cv::Rect ContainsTemplatePos(cv::Mat& input, cv::Mat& templ, double threshold) {
  assert(input.type() == CV_8UC1);
  assert(templ.type() == CV_8UC1);

  // Create the result matrix
  int result_cols = input.cols - templ.cols + 1;
  int result_rows = input.rows - templ.rows + 1;
  Mat result(result_cols, result_rows, CV_32FC1);

  // Match the template against the input image, and normalize resulting matrix
  matchTemplate(input, templ, result, CV_TM_CCOEFF);
  normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

  Mat playerName;
  input.copyTo(playerName);

  // Find the best match within the resized image and crop that matching area
  Point maxLoc;
  minMaxLoc(result, NULL, NULL, NULL, &maxLoc);

  rectangle( input, Point( maxLoc.x, maxLoc.y ), Point( maxLoc.x + templ.cols, maxLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  rectangle( result, maxLoc, Point( maxLoc.x + templ.cols , maxLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
  rectangle( playerName, Point( maxLoc.x, maxLoc.y - 80 ), Point( maxLoc.x + templ.cols + 45, maxLoc.y + templ.rows - 315 ), Scalar::all(0), 2, 8, 0 );

  char sb[ 100 ];
  sprintf(sb, "%s%d.png", "input", rand() % 10000);
  char sb3[ 100 ];
  sprintf(sb3, "%s%d.png", "character_name", rand() % 10000);

  imwrite(sb, input);
  imwrite(sb3, playerName);

  cv::Rect roi;
  roi.x = maxLoc.x;
  roi.y = maxLoc.y - 80;
  roi.width = (templ.cols - 45);
  roi.height = (templ.rows - 315) + 80;

  cout << (templ.cols - 45) << endl;
  cout << (templ.rows - 315) + 80 << endl;

  Mat resultImg;

  char sb2[ 100 ];
  sprintf(sb2, "%s%d.png", "resultImg", rand() % 10000);

  imwrite(sb2, playerName(roi));
  resultImg = playerName(roi);

  Ptr<OCRTesseract> ocr = OCRTesseract::create();
  string output;
  ocr->run(resultImg, output, NULL, NULL, NULL, OCR_LEVEL_WORD);

  cout << "OCR output: " << output << endl;

  Rect bounds(maxLoc.x, maxLoc.y, templ.cols, templ.rows);
  Mat cropped = input(bounds);

  // Determine the similarity between the cropped match and the template
  double similarity = GetMSSIM(cropped, templ)[0];
  cout << similarity << endl;

  if (similarity > threshold) {
    return bounds;
  } else {
    return Rect();
  }
}

cv::Rect ContainsTemplate(Mat& input, Mat& templ, double threshold) {
  return ContainsTemplatePos(input, templ, threshold);
}

// Get the character icons from a screenshot
vector<cv::Mat> ExtractCharacterIcons(cv::Mat &input) {
  vector<cv::Mat> icons;

  for(auto &pos : CharacterIconPositions) {
    icons.push_back(input(pos));
  }

  return icons;
}
