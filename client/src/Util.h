#ifndef util_h
#define util_h

#include <string>
#include <vector>
#include "glob.h"

std::vector<std::string> glob(const std::string& pattern);
std::string basename(const std::string& path);

#endif /* util_h */
