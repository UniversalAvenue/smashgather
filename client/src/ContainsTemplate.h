#ifndef CONTAINSTEMPLATE_H
#define CONTAINSTEMPLATE_H

#include <opencv2/core/core.hpp>
#include <exception>
#include <map>

// 60% structural similarity seems good enough based on my limited testing
const double MSSIM_THRESHOLD = 0.6;

// The positions of the character icons on the cropped and scaled winscreen.
// This is for different counts of players as the positions change based on that.
const std::map<int, const std::vector<cv::Rect>> CharacterIconPositions = {
  {2, {
    {490, 146, 40, 40},
    {790, 146, 40, 40}}
  },
  {3, {
    {454, 146, 40, 40},
    {640, 146, 40, 40},
    {827, 146, 40, 40}}
  },
  {4, {
    {415, 146, 40, 40},
    {565, 146, 40, 40},
    {715, 146, 40, 40},
    {865, 146, 40, 40}}
  },
};

void TrimBlackContour(cv::Mat&, cv::Mat&);

bool ContainsTemplate(cv::Mat&, cv::Mat&, double threshold = MSSIM_THRESHOLD);
cv::Rect ContainsTemplatePos(cv::Mat&, cv::Mat&, double threshold = MSSIM_THRESHOLD);

std::vector<cv::Mat> ExtractCharacterIcons(cv::Mat &, int player_count = 4);

#endif
