#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include "CharacterDetails.h"
#include "ContainsTemplate.h"
#include "WinDetector.h"

using namespace std;
using namespace cv;

bool IsWinScreen(Mat& screen) {
  Mat templ;
  cvtColor(imread("img/templates/wins_template.jpg"), templ, COLOR_BGR2GRAY);
  return ContainsTemplate(screen, templ);
}

bool ExtractWinner(Mat& screen, CharacterDetails& winner) {
  for (CharacterDetails character: CHARACTERS) {
    Mat templ;
    cvtColor(imread(character.template_filename), templ, COLOR_BGR2GRAY);
    if (ContainsTemplate(screen, templ)) {
      winner = character;
      return true;
    }
  }
  return false;
}

bool DetectWin(cv::Mat& screen, bool& is_winner_detected, CharacterDetails& winner) {
  bool is_win = false;
  is_winner_detected = false;
  is_win = IsWinScreen(screen);
  if (is_win) {
    is_winner_detected = ExtractWinner(screen, winner);
  }
  return is_win;
}
