#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "WinDetector.h"

using namespace std;
using namespace cv;

// For command line use
int main1(int argc, char* argv[])
{
  cout << endl << argv[0] << endl << endl;
  cout << "Analyzes an image to determine if it is a SSB64 win screen, and if so, which character is the winner." << endl;

  Mat input;
  if (argc > 1) {
    input = imread(argv[1]);
  } else {
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
    cout << "Did not detect a winner!" << endl;
    return 1;
  }
}
