#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <chrono>
#include <ctime>
#include "src/ContainsTemplate.h"

using namespace std;
using namespace cv;

// Test using training images
int main(int argc, char* argv[]) {

  if (argc == 1) {
    cout << "Given a screenshot, extract the players icons and save them to the current directory." << endl;
    cout << "Usage: " << string(argv[0]) << " " << "screenshot1.png screenshot2.png ..." << endl;
    return 1;
  }

  vector<Rect> icon_pos;
  icon_pos.push_back(Rect(415, 146, 40, 40));
  icon_pos.push_back(Rect(565, 146, 40, 40));
  icon_pos.push_back(Rect(715, 146, 40, 40));
  icon_pos.push_back(Rect(865, 146, 40, 40));

  for (size_t i = 1; i < argc; i++) {
    auto filename = string(argv[i]);
    auto contents = imread(filename);

    Mat screenshot;
    TrimBlackContour(contents, screenshot);

    for(auto &pos : icon_pos) {
      auto time = std::chrono::system_clock::now();
      auto output_filename = to_string(time.time_since_epoch().count()) + ".png";
      imwrite(output_filename, screenshot(pos));
    }
  }

  return 0;
}
