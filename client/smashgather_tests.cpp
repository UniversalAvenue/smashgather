#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "src/WinDetector.h"

using namespace std;
using namespace cv;

// Test using training images
int main(int argc, char* argv[]) {
  string filenames[] = {
    "img/tests/kirby_large.png",
    "img/tests/cfalcon.png",
    "img/tests/dk.png",
    "img/tests/fox.png",
    "img/tests/jigglypuff.png",
    "img/tests/kirby.png",
    "img/tests/link.png",
    "img/tests/mario.png",
    "img/tests/ness.png",
    "img/tests/pikachu.png",
    "img/tests/samus.png",
    "img/tests/yoshi.png",
    "img/tests/character_select.png",
    "img/tests/stage_select.png",
    "img/tests/fight_1.png",
    "img/tests/fight_2.png",
    "img/tests/fight_3.png",
    "img/tests/fight_4.png",
  };

  // Check files
  cout << "Running tests..." << endl;
  cout << "filename, IsWinScreen, GetCharacterMatch, Name" << endl;
  for (string filename: filenames) {
    Mat input;
    cvtColor(imread(filename), input, COLOR_BGR2GRAY);
    bool is_win = false, is_winner_detected = false;
    CharacterDetails winner("n/a", "");

    is_win = DetectWin(input, is_winner_detected, winner);
    cout << filename << ": " << is_win << ", " << is_winner_detected << ", " << winner.name << endl;
  }
  cout << "Done" << endl;
  return 0;
}
