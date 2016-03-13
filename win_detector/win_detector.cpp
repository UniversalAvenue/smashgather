#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cpr/cpr.h>

#include "src/CaptureScreenshot.h"
#include "src/WinDetector.h"

using namespace std;
using namespace cv;

static const string SERVER_URL = "http://localhost:5000/game";

bool PostWinnerData(string name) {
  string url = SERVER_URL;
  cout << "Start POST " << url << " { \"winner\": \"" << name << "\" }" << endl;
  auto request = cpr::Post(
      cpr::Url{url},
      cpr::Payload{{"winner", name}}
  );
  if (request.status_code == 200) {
    cout << "Finished POST " << url << endl;
    cout << "Response (code: " << request.status_code << "):" << endl;
    cout << "  " << request.text << endl;
    return true;
  } else {
    cout << "Failed POST " << url << " (code: " << request.status_code << ")" << endl;
    cout << "Response (code: " << request.status_code << "):" << endl;
    cout << "  " << request.text << endl;
    return false;
  }
}

enum WinDetectorState { INIT, GAME, WIN_DETECTED, WINNER_IDENTIFIED, GAME_SAVED };

int poll() {
  WinDetectorState state = WinDetectorState::INIT;
  cout << "WinDetector: start polling!" << endl;
  while (true) {
    // Run detection on current screen
    auto screen = CaptureScreenshot();
    bool is_win = false, is_winner_detected = false;
    CharacterDetails winner("n/a", "");
    is_win = DetectWin(screen, is_winner_detected, winner);

    // Run state machine
    // TODO: need game screen detection to avoid spurious win detections
    if (is_win && state != WinDetectorState::WINNER_IDENTIFIED) {
      state = WinDetectorState::WIN_DETECTED;
      cout << "DETECTED WIN! ";
      if (is_winner_detected) {
        state = WinDetectorState::WINNER_IDENTIFIED;
        cout << winner.name << "!" << endl;
        PostWinnerData(winner.name);
      } else {
        cout << "Could not detect winner." << endl;
      }
    } else {
      state = WinDetectorState::GAME;
    }
    sleep(1);
  }
  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    return poll();
  } else if (argc == 3 && strcmp(argv[1], "-i") == 0) {
    Mat input;
    cvtColor(imread(argv[2]), input, COLOR_BGR2GRAY);
    bool is_win = false, is_winner_detected = false;
    CharacterDetails winner("n/a", "");

    is_win = DetectWin(input, is_winner_detected, winner);

    if (is_win && is_winner_detected) {
      cout << "Winner: " << winner.name << endl;
      return 0;
    } else if (is_win) {
      cout << "Detected win, but could not identify winner!" << endl;
      return 0;
    } else {
      cout << "Did not detect a win!" << endl;
      return 1;
    }
  } else {
    // TODO: update usage to include polling mode
    cout << "Analyzes an image to determine if it is a SSB64 win screen, and if so, which character is the winner." << endl;
    cout << "    Usage: " << argv[0] << " <input_image>" << endl;
    return 0;
  }
}
