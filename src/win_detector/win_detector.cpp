#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "src/WinDetector.h"

using namespace std;
using namespace cv;

// For command line use
int main(int argc, char* argv[]) {
  Mat input;
  if (argc > 1) {
    input = imread(argv[1]);
  } else {
    cout << "Analyzes an image to determine if it is a SSB64 win screen, and if so, which character is the winner." << endl;
    cout << "    Usage: " << argv[0] << " <input_image>" << endl;
    return 0;
  }

  bool is_win = false, is_winner_detected = false;
  CharacterDetails winner("n/a", "");

  is_win = DetectWin(input, is_winner_detected, winner);

  if (is_win && is_winner_detected) {
    cout << "Winner: " << winner.name << endl;
    return 0;
  } else if (is_win) {
    cout << "Detected win, but could not identify winner!" << endl;
    return 0;
  } else {
    cout << "Did not detect a win!" << endl;
    return 1;
  }
}
