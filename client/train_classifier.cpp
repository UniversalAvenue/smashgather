#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>

#include <iostream>
#include <algorithm>

#include "src/Classifier.hpp"
#include "src/Util.hpp"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {

  if (argc != 2) {
    cout << "Usage: " << argv[0] << " train|test" << endl;
    cout << "Trains the classifier using data from training_data/* and writes the resulting SVM\n";
    cout << "to multi-class.svm. In test mode the same file is read and ran against all samples.\n";
    return 1;
  }

  string action(argv[1]);

  if (action == "train") {

    // Get all the character names in the training data directory.
    auto characters = glob("./training_data/*");
    transform(characters.begin(), characters.end(), characters.begin(), basename);

    // Init the training data matrix and fill it with the samples while also adding the correct
    // labels to the labels matrix.
    Mat training_data(0, 40*40*3, CV_32FC1);
    Mat labels(0, 1, CV_32FC1);

    for(auto &character : characters) {
      // The class id is simply the index of the character name in Classifier::characters
      auto it = find(Classifier::characters.begin(), Classifier::characters.end(), character);
      if (it == Classifier::characters.end()) {
        cout << "Unknown character in ./training_data: " << character << endl;
        return 1;
      }
      auto class_id = (double) distance(Classifier::characters.begin(), it);

      for(auto &filename : glob("./training_data/" + character + "/*.png")) {
        auto image = imread(filename);

        training_data.push_back(Classifier::convert_image(image));
        labels.push_back(Mat(1, 1, CV_32FC1, {class_id}));
      }
    }

    CvSVMParams params;
    params.svm_type    = SVM::C_SVC;
    params.C           = 0.1;
    params.kernel_type = SVM::LINEAR;
    params.term_crit   = TermCriteria(CV_TERMCRIT_ITER, (int)1e7, 1e-6);

    cout << endl << "Training the classifier.. This will take a minute." << endl;

    CvSVM svm;
    svm.train(training_data, labels, Mat(), Mat(), params);
    svm.save("multi-class.svm");

    cout << "Done." << endl;

  } else if (action == "test") {

    Classifier classifier("multi-class.svm");

    for(auto &filename : glob("./training_data/*/*.png")) {
      auto label = classifier.classify(imread(filename));
      cout << filename << " " << label << endl;
    }

  } else {
    cout << "Unknown option: " << action << endl;
    return 1;
  }

  return 0;
}
