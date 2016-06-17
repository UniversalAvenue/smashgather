#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "src/WinDetector.h"
#include "src/ContainsTemplate.h"
#include "src/Util.h"

using namespace std;
using namespace cv;

// Test using training images
int main(int argc, char* argv[]) {
  auto filenames = glob("img/tests/*.png");

  // Check files
  cout << "Running tests..." << endl;
  cout << "filename, IsWinScreen, GetCharacterMatch, Name" << endl;
  for (string filename: filenames) {
    try {
      Mat input = imread(filename);
      bool is_win = false, is_winner_detected = false;
      CharacterDetails winner("n/a");

      is_win = DetectWin(input, is_winner_detected, winner);
      cout << filename << ": " << is_win << ", " << is_winner_detected << ", " << winner.name << endl;
    } catch (const exception &e) {
      cout << filename << ": " << e.what() << endl;
    }
  }
  cout << "Done" << endl;
  return 0;
}
