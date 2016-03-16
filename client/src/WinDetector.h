#ifndef WINDETECTOR_H
#define WINDETECTOR_H

#include <opencv2/core/core.hpp>
#include <iostream>

#include "CharacterDetails.h"

bool IsWinScreen(cv::Mat& screen);
bool ExtractWinner(cv::Mat& win_screen, CharacterDetails& winner);
bool DetectWin(cv::Mat& screen, bool& is_winner_detected, CharacterDetails& winner);

#endif
