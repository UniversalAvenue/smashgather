// GraphQL object types
class Character {}
class User {}
class Game {}

function getCharacter(id) {
  return {
    name: "Ness",
  }
}

function getUser(id) {
  return {
    name: "Neville",
    username: "@neville",
    character: getCharacter(),
  }
}

function getGame(id) {
  return {
    createdAt: "18:52:55.293953-04",
    user: getUser(),
    character: getCharacter(),
    verified: false,
  }
}

function getCharacters() {
  console.log("getCharacters")
  return [getCharacter()];
}

function getUsers() {
  console.log("getUsers")
  return [getUser()];
}

function getGames() {
  console.log("getGames")
  return [getGame()];
}

function createGame({ characterName }) {
  console.log(`createGame, characterName: ${characterName}`)
  // NOTE: when this is implemented, don't forget to create a sequential game ID
  return getGame()
}

module.exports = {
  getCharacter,
  getUser,
  getGame,
  getCharacters,
  getUsers,
  getGames,
  createGame,
  Character,
  User,
  Game,
};
