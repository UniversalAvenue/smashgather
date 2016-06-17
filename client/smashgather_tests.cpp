#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <algorithm>
#include "src/WinDetector.h"
#include "src/ContainsTemplate.h"
#include "src/Util.h"

using namespace std;
using namespace cv;

// Test using training images
int main(int argc, char* argv[]) {
  auto filenames = glob("img/tests/*.png");

  // Check files
  cout << "Running tests..." << endl;
  cout << "filename, IsWinScreen, GetCharacterMatch, Name" << endl;
  for (string filename: filenames) {
    try {
      Mat input = imread(filename);
      auto players = DetectWins(input);

      if (players.size() > 0) {
        vector<string> names;
        names.resize(players.size());

        transform(players.begin(), players.end(), names.begin(),
          [](CharacterDetails &player){
            return player.name;
          });

        cout << filename << ": ";
        copy(names.begin(), names.end(), ostream_iterator<string>(cout, " "));
        cout << endl;
      } else {
        cout << filename << ": NO_WINNER_DETECTED\n";
      }
    } catch (const exception &e) {
      cout << filename << ": " << e.what() << endl;
    }
  }
  cout << "Done" << endl;
  return 0;
}
