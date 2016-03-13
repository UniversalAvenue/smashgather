#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "WinDetector.h"

using namespace std;
using namespace cv;

// Test using training images
int main(int argc, char* argv[])
{
  string filenames[] = {
    "img/cfalcon.png",
    "img/dk.png",
    "img/fox.png",
    "img/jigglypuff.png",
    "img/kirby.png",
    "img/link.png",
    "img/mario.png",
    "img/ness.png",
    "img/pikachu.png",
    "img/samus.png",
    "img/yoshi.png",
    "img/character_select.png",
    "img/stage_select.png",
    "img/fight_1.png",
    "img/fight_2.png",
    "img/fight_3.png",
    "img/fight_4.png",
  };

  // Check files
  cout << "Running tests..." << endl;
  cout << "filename, IsWinScreen, GetCharacterMatch, Name" << endl;
  for (string filename: filenames) {
    Mat input = imread(filename);
    bool is_win = false, is_winner_detected = false;
    CharacterDetails winner("n/a", "");

    is_win = DetectWin(input, is_winner_detected, winner);
    cout << filename << ": " << is_win << ", " << is_winner_detected << ", " << winner.name << endl;
  }
  cout << "Done" << endl;
  return 0;
}
