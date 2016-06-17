#ifndef classifier_h
#define classifier_h

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>

#include "CharacterDetails.h"

// Exception thrown when classifier failed to load the params
class InvalidClassifier: public std::exception {
  const char* what() const throw() {
    return "The classifier couldn't be loaded";
  }
};

class Classifier {
  cv::Ptr<cv::ml::SVM> svm;

public:
  Classifier(std::string filename) {
    svm = cv::Algorithm::load<cv::ml::SVM>(filename);
    if(svm->empty()) {
      throw InvalidClassifier();
    }
  }

  // Guess the characters name from the image icon.
  const CharacterDetails& classify(const cv::Mat &image) {
    auto sample = Classifier::convert_image(image);
    auto class_id = (size_t) svm->predict(sample);
    return CHARACTERS[class_id];
  }

  // Get the class id from a character name. It's simply the index of the character in CHARACTERS.
  static int class_id(std::string character) {
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
  static cv::Mat convert_image(const cv::Mat &image) {
    assert(image.type() == CV_8UC3);
    assert(image.rows == 40);
    assert(image.cols == 40);

    cv::Mat copy;
    cv::cvtColor(image, copy, CV_BGR2HSV);
    copy.reshape(1, 1).convertTo(copy, CV_32F, (double)1 / 255);

    return copy;
  }
};

#endif /* classifier_h */
