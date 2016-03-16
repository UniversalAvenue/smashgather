#ifndef CONTAINSTEMPLATE_H
#define CONTAINSTEMPLATE_H

#include <opencv2/core/core.hpp>

// 60% structural similarity seems good enough based on my limited testing
const double MSSIM_THRESHOLD = 0.6;

bool ContainsTemplate(cv::Mat&, cv::Mat&, double threshold = MSSIM_THRESHOLD);

#endif
