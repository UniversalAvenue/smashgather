#ifndef classifier_h
#define classifier_h

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>

#include "CharacterDetails.h"

using namespace std;
using namespace cv;

class Classifier {
  CvSVM svm;

public:
  Classifier(string filename) {
    svm.load(filename.c_str());
  }

  // Guess the characters name from the image icon.
  const CharacterDetails& classify(const Mat &image) {
    auto class_id = (size_t) svm.predict(Classifier::convert_image(image));
    return CHARACTERS[class_id];
  }

  // Get the class id from a character name. It's simply the index of the character in CHARACTERS.
  static int class_id(string character) {
    auto idx = 0;
    for(auto &CHAR : CHARACTERS) {
      if (CHAR.name == character) {
        return idx;
      } else {
        idx++;
      }
    }

    return -1;
  }

  // Convert the input image to something we can run the classifier on. Using HSV as I suspect it
  // might help the SVM with classification. We also "unroll" it and convert to floating point
  // with appropriate scaling.
  static Mat convert_image(const Mat &image) {
    assert(image.type() == CV_8UC3);
    assert(image.rows == 40);
    assert(image.cols == 40);

    Mat copy;
    cvtColor(image, copy, CV_BGR2HSV);
    copy.reshape(1, 1).convertTo(copy, CV_32FC1, (double)1 / 255);

    return copy;
  }
};

#endif /* classifier_h */
