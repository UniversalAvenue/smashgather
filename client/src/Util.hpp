#ifndef util_h
#define util_h

#include "glob.h"

using namespace std;

vector<string> glob(const string& pattern){
    glob_t glob_result;
    glob(pattern.c_str(),GLOB_TILDE,NULL,&glob_result);
    vector<string> files;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        files.push_back(string(glob_result.gl_pathv[i]));
    }
    globfree(&glob_result);
    return files;
}

string basename(const string& path) {
  return path.substr(path.find_last_of("/\\") + 1);
}

#endif /* util_h */
