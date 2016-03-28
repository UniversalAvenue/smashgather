#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <ApplicationServices/ApplicationServices.h>

#include "CaptureScreenshot.h"

using namespace cv;

// Adapted from http://docs.opencv.org/2.4/doc/tutorials/ios/image_manipulation/image_manipulation.html
Mat ConvertToMatrix(const CGImageRef& image) {
  auto color_space = CGImageGetColorSpace(image);
  auto cols = CGImageGetWidth(image);
  auto rows = CGImageGetHeight(image);

  Mat matrix(rows, cols, CV_8UC4); // 8 bits per component, 4 channels (color channels + alpha)

  auto context = CGBitmapContextCreate(
      matrix.data,                // Pointer to  data
      cols,                       // Width of bitmap
      rows,                       // Height of bitmap
      8,                          // Bits per component
      matrix.step[0],             // Bytes per row
      color_space,                // Colorspace
      kCGImageAlphaNoneSkipLast | kCGBitmapByteOrderDefault // Bitmap info flags
  );

  CGContextDrawImage(context, CGRectMake(0, 0, cols, rows), image);
  CGContextRelease(context);

  // Convert to grayscale format
  Mat gray(rows, cols, CV_8UC1);
  cvtColor(matrix, gray, COLOR_BGR2GRAY);

  return gray;
}

Mat CaptureScreenshot() {
  auto display_ID = CGMainDisplayID();
  auto display_bounds = CGDisplayBounds(display_ID);
  auto height = CGRectGetHeight(display_bounds);
  auto width = CGRectGetWidth(display_bounds);
  auto image = CGDisplayCreateImageForRect(display_ID, CGRectMake(0, 0, width, height));
  Mat image_mat = ConvertToMatrix(image);
  CFRelease(image);
  return image_mat;
}
