#include <cpr/cpr.h>
#include <iostream>
#include <sstream>

#include "NetworkLayer.h"

using namespace std;

static string server_url = "http://localhost:8080/graphql";
static string token = "";
static int client_mutation_id = 0;

bool InitNetworkLayer(string _server_url, string _token) {
  // Initialize our client state
  server_url = _server_url;
  token = _token;
  client_mutation_id = 0;

  // Execute a simple query to check connectivity
  string query = "query { viewer { games(first: 1) { edges { node { createdAt, character { name } } } } } }";
  return RunGraphqlQuery(query);
}

bool RunGraphqlQuery(const string& query) {
  return RunGraphqlQuery(query, "");
}

bool RunGraphqlQuery(const string& query, const string& screenshot) {
  cout << "Start POST " << server_url << ": " + query << endl;
  cpr::Response request;
  if (screenshot.length() == 0) {
    request = cpr::Post(
        cpr::Url{server_url},
        cpr::Header{{"Authorization", "Bearer " + token}},
        cpr::Payload{{"query", query}}
    );
  } else {
    request = cpr::Post(
        cpr::Url{server_url},
        cpr::Header{{"Authorization", "Bearer " + token}},
        cpr::Multipart{{"query", query}, {"screenshot", cpr::File{screenshot}}}
    );
  };
  if (request.status_code == 200) {
    cout << "Finished POST " << server_url << endl;
    cout << "Response (code: " << request.status_code << "):" << endl;
    cout << "  " << request.text << endl;
    return true;
  } else {
    cout << "Failed POST " << server_url << " (code: " << request.status_code << ")" << endl;
    cout << "Response (code: " << request.status_code << "):" << endl;
    cout << "  " << request.text << endl;
    return false;
  }
}

bool RunCreateGameMutation(const CharacterDetails& winner, const string& screenshot) {
  const string name = winner.name;
  ostringstream oss;
  oss << "mutation create_game { createGame(input: { "
    << "characterName: \"" << name << "\", "
    << "clientMutationId: \"" << client_mutation_id << "\""
    << "}) { game { createdAt, user { name }, character { name } } } }";
  string query = oss.str();
  ++client_mutation_id;
  return RunGraphqlQuery(query, screenshot);
}
