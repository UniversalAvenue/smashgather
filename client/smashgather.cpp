#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "src/CaptureScreenshot.h"
#include "src/NetworkLayer.h"
#include "src/WinDetector.h"

using namespace std;
using namespace cv;

static const char* DEFAULT_SERVER_URL = "http://localhost:8080/graphql";
static const char* DEFAULT_TOKEN = "";
enum SmashgatherState { INIT, GAME, WIN_DETECTED, GAME_SAVED };

int loop() {
  SmashgatherState state = SmashgatherState::INIT;
  while (true) {
    // Run detection on current screen
    auto screen = CaptureScreenshot();
    bool is_win = false, is_winner_detected = false;
    CharacterDetails winner("n/a", "");
    is_win = DetectWin(screen, is_winner_detected, winner);

    // Run state machine
    switch (state) {
      case SmashgatherState::INIT:
        // TODO: need game screen detection to avoid spurious win detections
        // immediately fallthrough to GAME state
        cout << "STATE: GAME" << endl;
        state = SmashgatherState::GAME;
      case SmashgatherState::GAME:
        // wait in this state until we detect a win screen -> WIN_DETECTED
        if (is_win) {
          cout << "STATE: WIN_DETECTED" << endl;
          state = SmashgatherState::WIN_DETECTED;
          // intentional fallthrough to next state
        } else {
          break;
        }
      case SmashgatherState::WIN_DETECTED:
        // wait in this state until we either:
        // 1) detect a winner -> save game, then go to GAME_SAVED
        // 2) no longer detect win screen -> log error, go to GAME
        if (is_win) {
          if (is_winner_detected) {
            cout << "Detected winner: " << winner.name << "!" << endl;
            RunCreateGameMutation(winner);
            cout << "STATE: GAME_SAVED" << endl;
            state = SmashgatherState::GAME_SAVED;
          } else {
            break;
          }
        } else {
          // TODO: record unknown winner?
          cout << "Detected win, but couldn't detect winner!";
          state = SmashgatherState::GAME;
          break;
        }
        break;
      case SmashgatherState::GAME_SAVED:
        // wait in this state until we stop detecting a win screen -> GAME
        if (!is_win) {
          cout << "STATE: GAME" << endl;
          state = SmashgatherState::GAME;
        }
        break;
    }

    // Sleep before looping again
    sleep(1);
  }
  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 1) {
    // Run main loop
    string banner =
    "==================================================================================\n"
    "   _____ __  __           _____ _    _  _____       _______ _    _ ______ _____  \n"
    "  / ____|  \\/  |   /\\    / ____| |  | |/ ____|   /\\|__   __| |  | |  ____|  __ \\ \n"
    " | (___ | \\  / |  /  \\  | (___ | |__| | |  __   /  \\  | |  | |__| | |__  | |__) |\n"
    "  \\___ \\| |\\/| | / /\\ \\  \\___ \\|  __  | | |_ | / /\\ \\ | |  |  __  |  __| |  _  / \n"
    "  ____) | |  | |/ ____ \\ ____) | |  | | |__| |/ ____ \\| |  | |  | | |____| | \\ \\ \n"
    " |_____/|_|  |_/_/    \\_\\_____/|_|  |_|\\_____/_/    \\_\\_|  |_|  |_|______|_|  \\_\\\n"
    "==================================================================================\n";
    cout << banner;
    cout << "Connecting to Smashgather server..." << endl;
    const char* pUrl = getenv("SERVER_URL");
    if (pUrl == nullptr) {
      pUrl = DEFAULT_SERVER_URL;
    }
    const char* pToken = getenv("TOKEN");
    if (pToken == nullptr) {
      pToken = DEFAULT_TOKEN;
    }
    if (InitNetworkLayer(string(pUrl), string(pToken))) {
      cout << "Connected! Running Smashgather (use CTRL+C to exit)..." << endl;
      return loop();
    } else {
      cout << "Failed to connect to Smashgather server. Exiting..." << endl;
      return 1;
    }
  } else if (argc == 3 && strcmp(argv[1], "-i") == 0) {
    // Analyze input image
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
    cout << "Monitors screen output to detect SSB64 win screens, and records winner to the Smashgather server." << endl;
    cout << "Or, when given an input image, analyzes it to detect a win screen and outputs winner if detected." << endl;
    cout << "    Usage: " << argv[0] << " [-i <input_image>]" << endl;
    return 0;
  }
}
