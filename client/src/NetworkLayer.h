#ifndef NETWORKLAYER_H
#define NETWORKLAYER_H

#include <iostream>

#include "CharacterDetails.h"

bool InitNetworkLayer(std::string server_url, std::string token);

bool RunGraphqlQuery(const std::string& query);

bool RunCreateGameMutation(const CharacterDetails& winner);

#endif
