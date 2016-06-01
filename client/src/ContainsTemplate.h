#ifndef CONTAINSTEMPLATE_H
#define CONTAINSTEMPLATE_H

#include <opencv2/core/core.hpp>
#include <exception>

// 60% structural similarity seems good enough based on my limited testing
const double MSSIM_THRESHOLD = 0.6;

// The positions of the character icons on the cropped and scaled winscreen.
const std::vector<cv::Rect> CharacterIconPositions = {
  {415, 146, 40, 40},
  {565, 146, 40, 40},
  {715, 146, 40, 40},
  {865, 146, 40, 40},
};

void TrimBlackContour(cv::Mat&, cv::Mat&);

bool ContainsTemplate(cv::Mat&, cv::Mat&, double threshold = MSSIM_THRESHOLD);
cv::Rect ContainsTemplatePos(cv::Mat&, cv::Mat&, double threshold = MSSIM_THRESHOLD);

std::vector<cv::Mat> ExtractCharacterIcons(cv::Mat &);

#endif
