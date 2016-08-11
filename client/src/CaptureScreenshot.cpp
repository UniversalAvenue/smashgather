#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "CaptureScreenshot.h"

using namespace cv;

Mat CaptureScreenshot() {
  VideoCapture cap;
  cap.set(CV_CAP_PROP_BUFFERSIZE, 3);
  // open the default camera, use something different from 0 otherwise;
  // Check VideoCapture documentation.
  if(!cap.open(0))
      return *new Mat();

  Mat frame;
  cap >> frame;
  return frame;
}
