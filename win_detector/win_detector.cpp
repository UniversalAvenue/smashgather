#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cpr/cpr.h>
#include <iostream>
#include <sstream>

#include "src/CaptureScreenshot.h"
#include "src/WinDetector.h"

using namespace std;
using namespace cv;

static const string SERVER_URL = "http://localhost:8080/graphql";

int client_mutation_id = 0;

bool RunCreateGameMutation(string name) {
  string url = SERVER_URL;
  ostringstream oss;
  oss << "mutation create_game { createGame(input: { "
    << "characterName: \"" << name << "\", "
    << "clientMutationId: \"" << client_mutation_id << "\""
    << "}) { game { createdAt, user { name }, character { name } } } }";
  string graphql_query = oss.str();
  ++client_mutation_id;
  cout << "Start POST " << url << ": " + graphql_query << endl;
  auto request = cpr::Post(
      cpr::Url{url},
      cpr::Payload{{"query", graphql_query}}
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

enum WinDetectorState { INIT, GAME, WIN_DETECTED, GAME_SAVED };

int loop() {
  WinDetectorState state = WinDetectorState::INIT;
  string banner =
  "==================================================================================\n"
  "   _____ __  __           _____ _    _  _____       _______ _    _ ______ _____  \n"
  "  / ____|  \\/  |   /\\    / ____| |  | |/ ____|   /\\|__   __| |  | |  ____|  __ \\ \n"
  " | (___ | \\  / |  /  \\  | (___ | |__| | |  __   /  \\  | |  | |__| | |__  | |__) |\n"
  "  \\___ \\| |\\/| | / /\\ \\  \\___ \\|  __  | | |_ | / /\\ \\ | |  |  __  |  __| |  _  / \n"
  "  ____) | |  | |/ ____ \\ ____) | |  | | |__| |/ ____ \\| |  | |  | | |____| | \\ \\ \n"
  " |_____/|_|  |_/_/    \\_\\_____/|_|  |_|\\_____/_/    \\_\\_|  |_|  |_|______|_|  \\_\\\n"
  "==================================================================================\n";
  cout << banner << "Running win detector (use CTRL+C to exit)..." << endl;
  while (true) {
    // Run detection on current screen
    auto screen = CaptureScreenshot();
    bool is_win = false, is_winner_detected = false;
    CharacterDetails winner("n/a", "");
    is_win = DetectWin(screen, is_winner_detected, winner);

    // Run state machine
    switch (state) {
      case WinDetectorState::INIT:
        // TODO: need game screen detection to avoid spurious win detections
        // immediately fallthrough to GAME state
        cout << "STATE: GAME" << endl;
        state = WinDetectorState::GAME;
      case WinDetectorState::GAME:
        // wait in this state until we detect a win screen -> WIN_DETECTED
        if (is_win) {
          cout << "STATE: WIN_DETECTED" << endl;
          state = WinDetectorState::WIN_DETECTED;
          // intentional fallthrough to next state
        } else {
          break;
        }
      case WinDetectorState::WIN_DETECTED:
        // wait in this state until we either:
        // 1) detect a winner -> save game, then go to GAME_SAVED
        // 2) no longer detect win screen -> log error, go to GAME
        if (is_win) {
          if (is_winner_detected) {
            cout << "Detected winner: " << winner.name << "!" << endl;
            RunCreateGameMutation(winner.name);
            cout << "STATE: GAME_SAVED" << endl;
            state = WinDetectorState::GAME_SAVED;
          } else {
            break;
          }
        } else {
          // TODO: record unknown winner?
          cout << "Detected win, but couldn't detect winner!";
          state = WinDetectorState::GAME;
          break;
        }
        break;
      case WinDetectorState::GAME_SAVED:
        // wait in this state until we stop detecting a win screen -> GAME
        if (!is_win) {
          cout << "STATE: GAME" << endl;
          state = WinDetectorState::GAME;
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
    return loop();
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
    cout << "Monitors screen output to detect SSB64 win screens, and records winner to the Smashgather server." << endl;
    cout << "Or, when given an input image, analyzes it to detect a win screen and outputs winner if detected." << endl;
    cout << "    Usage: " << argv[0] << " [-i <input_image>]" << endl;
    return 0;
  }
}
