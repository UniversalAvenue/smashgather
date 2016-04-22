#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ContainsTemplate.h"

using namespace std;
using namespace cv;

#include <ctime>

// Save the image locally and return the filename
std::string SaveWinScreen(Mat& screen) {
  system("mkdir -p screenshots");
  auto at = time(nullptr);
  auto filename = "./screenshots/" + to_string(at) + ".png";
  imwrite(filename, screen);
  return filename;
}
