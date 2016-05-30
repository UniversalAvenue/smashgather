#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include "Classifier.hpp"
#include "CharacterDetails.h"
#include "ContainsTemplate.h"
#include "WinDetector.h"
#include "Util.hpp"

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

  // Create a vector of places
  vector<Place> places;

  // Convert to grayscale first
  Mat gray;
  cvtColor(screen, gray, COLOR_BGR2GRAY);

  // Fill it with matched locations of the place templates
  int place_num = 1;
  for (auto &place_template : glob("img/templates/*_place.png")) {
    Mat templ;
    cvtColor(imread(place_template), templ, COLOR_BGR2GRAY);
    auto pos = ContainsTemplatePos(gray, templ, 0.3);

    if (pos.area() == 0) {
      cerr << "Template " << place_template << " did not match anything in the win screen\n";
      return result;
    }

    places.push_back({nullptr, place_num++, pos});
  }

  // Now sort it by the X coord of the position
  sort(places.begin(), places.end(), [](Place a, Place b){ return a.pos.x < b.pos.x; });

  Classifier classifier("multi-class.svm");

  // Add the characters to their corresponding places
  auto place = places.begin();
  for (auto &icon : ExtractCharacterIcons(screen)) {
    place->character = &classifier.classify(icon);
    ++place;
  }

  // Finally, sort by the original place_num to get the final order
  sort(places.begin(), places.end(), [](Place a, Place b){ return a.place < b.place; });

  for (auto &place : places) {
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
