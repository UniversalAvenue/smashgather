#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include "Classifier.hpp"
#include "CharacterDetails.h"
#include "ContainsTemplate.h"
#include "WinDetector.h"
#include "Util.h"

using namespace std;
using namespace cv;

bool IsWinScreen(Mat& screen) {

  // Convert to grayscale first
  Mat gray;
  cvtColor(screen, gray, COLOR_BGR2GRAY);

  Mat templ;
  cvtColor(imread("img/templates/KOs.png"), templ, COLOR_BGR2GRAY);
  return ContainsTemplate(gray, templ);
}

vector<CharacterDetails> ExtractWinners(cv::Mat& screen) {

  vector<CharacterDetails> result;

  struct Place {
    const CharacterDetails *character;
    int place;
    Rect pos;
  };

  vector<Place> positions;

  // Convert to grayscale first
  Mat gray;
  cvtColor(screen, gray, COLOR_BGR2GRAY);

  // Fill it with matched locations of the place templates
  int place_num = 1;

  for (auto &place_template : glob("img/templates/*_place.png")) {
    Mat templ;
    cvtColor(imread(place_template), templ, COLOR_BGR2GRAY);
    auto pos = ContainsTemplatePos(gray, templ);

    if (pos.area() == 0) {
      if (place_num == 1 || place_num == 2) {
        // If we didn't find the first / second place template, this is not a valid win screen
        return result;
      }

      continue;
    }

    positions.push_back({nullptr, place_num++, pos});
  }

  // Sort the positions by their X coordinate to match with the character icons
  sort(positions.begin(), positions.end(), [](Place a, Place b){ return a.pos.x < b.pos.x; });

  Classifier classifier("multi-class.svm");

  // Add the characters to their corresponding positions
  auto place = positions.begin();
  for (auto &icon : ExtractCharacterIcons(screen, positions.size())) {
    place->character = &classifier.classify(icon);
    ++place;
  }

  // Finally, sort by the original place_num to get the final order
  sort(positions.begin(), positions.end(), [](Place a, Place b){ return a.place < b.place; });

  for (auto &place : positions) {
    result.push_back(*(place.character));
  }

  return result;
}

bool ExtractWinner(Mat& screen, CharacterDetails& winner) {
  auto results = ExtractWinners(screen);
  if (results.size() == 0) {
    return false;
  } else {
    winner = results[0];
    return true;
  }
}

bool DetectWin(cv::Mat& screen, bool& is_winner_detected, CharacterDetails& winner) {
  Mat resized;
  bool is_win = false;
  is_winner_detected = false;

  try {
    // Trim the black contour and resize the input
    TrimBlackContour(screen, resized);
  } catch (...) {
    return false;
  }

  is_win = IsWinScreen(resized);
  if (is_win) {
    is_winner_detected = ExtractWinner(resized, winner);
  }
  return is_win;
}

// Detect winners and return a list of characters corresponding to their placement.
// Return empty if no winner was detected.
vector<CharacterDetails> DetectWins(cv::Mat& screen) {
  Mat resized;

  try {
    // Trim the black contour and resize the input
    TrimBlackContour(screen, resized);
  } catch (...) {
    return {};
  }

  return ExtractWinners(resized);
}
