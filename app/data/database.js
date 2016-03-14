var pg = require("pg")
var databaseUrl = "pg://localhost/smashboard"

// GraphQL object types
class Character {}
class User {}
class Game {}

function getCharacter(id) {
  console.log("getCharacter", id)
  return new Promise((resolve, reject) => {
    pg.connect(databaseUrl, (err, client, done) => {
      client.query("SELECT * FROM characters WHERE id = $1", [id], (err, result) => {
        if (err) {
          reject(`Error in getCharacter(): ${err}`)
          done()
          return
        }
        if (result.rows.length < 1) {
          resolve(null)
          done()
          return
        }
        let row = result.rows[0]
        resolve(Object.assign(new Character(), {
          id: row.id,
          name: row.name,
        }))
        done()
      })
    })
  })
}

function getUser(id) {
  console.log("getUser", id)
  return new Promise((resolve, reject) => {
    pg.connect(databaseUrl, (err, client, done) => {
      client.query(
          `SELECT users.id, users.name, users.username, users.character_id, characters.name AS character_name
            FROM users
            LEFT JOIN characters ON users.character_id = characters.id
            WHERE users.id = $1;`,
          [id],
          (err, result) => {
        if (err) {
          reject(`Error in getUser(): ${err}`)
          done()
          return
        }
        if (result.rows.length < 1) {
          resolve(null)
          done()
          return
        }
        let row = result.rows[0]
        resolve(Object.assign(new User(), {
          id: row.id,
          name: row.name,
          username: row.username,
          character: {
            id: row.character_id,
            name: row.character_name
          }
        }))
        done()
      })
    })
  })
}

function getGame(id) {
  console.log("getGame", id)
  return new Promise((resolve, reject) => {
    pg.connect(databaseUrl, (err, client, done) => {
      client.query(
          `SELECT games.id, games.created_at, games.user_id, games.character_id, games.verified,
              users.name AS user_name, users.username AS user_username, users.character_id AS user_character_id,
              user_characters.name AS user_character_name,
              characters.name AS character_name
            FROM games
            LEFT JOIN users ON games.user_id = users.id
            LEFT JOIN characters AS user_characters on users.character_id = user_characters.id
            LEFT JOIN characters on games.character_id = characters.id
            WHERE games.id = $1;`,
          [id],
          (err, result) => {
        if (err) {
          reject(`Error in getGame(): ${err}`)
          done()
          return
        }
        if (result.rows.length < 1) {
          resolve(null)
          done()
          return
        }
        let row = result.rows[0]
        resolve(Object.assign(new Game(), {
          id: row.id,
          createdAt: row.created_at,
          verified: row.verified,
          character: {
            id: row.character_id,
            name: row.character_name
          },
          user: {
            id: row.user_id,
            name: row.user_name,
            username: row.user_username,
            character: {
              id: row.user_character_id,
              name: row.user_character_name
            }
          }
        }))
        done()
      })
    })
  })
}

function getCharacters() {
  console.log("getCharacters")
  return new Promise((resolve, reject) => {
    pg.connect(databaseUrl, (err, client, done) => {
      client.query("SELECT * FROM characters", (err, result) => {
        if (err) {
          reject(`Error in getCharacters(): ${err}`)
          done()
          return
        }
        let characters = result.rows.map((row) => {
          return {
            id: row.id,
            name: row.name,
          }
        })
        resolve(characters)
        done()
      })
    })
  })
}

function getUsers() {
  console.log("getUsers")
  return new Promise((resolve, reject) => {
    pg.connect(databaseUrl, (err, client, done) => {
      client.query(
          `SELECT users.id, users.name, users.username, users.character_id, characters.name AS character_name
            FROM users
            LEFT JOIN characters ON users.character_id = characters.id;`,
          (err, result) => {
        if (err) {
          reject(`Error in getUsers(): ${err}`)
          done()
          return
        }
        let users = result.rows.map((row) => {
          return {
            id: row.id,
            name: row.name,
            username: row.username,
            character: {
              id: row.character_id,
              name: row.character_name
            }
          }
        })
        resolve(users)
        done()
      })
    })
  })
}

function getGames() {
  console.log("getGames")
  return new Promise((resolve, reject) => {
    pg.connect(databaseUrl, (err, client, done) => {
      client.query(
          `SELECT games.id, games.created_at, games.user_id, games.character_id, games.verified,
              users.name AS user_name, users.username AS user_username, users.character_id AS user_character_id,
              user_characters.name AS user_character_name,
              characters.name AS character_name
            FROM games
            LEFT JOIN users ON games.user_id = users.id
            LEFT JOIN characters AS user_characters on users.character_id = user_characters.id
            LEFT JOIN characters on games.character_id = characters.id;`,
          (err, result) => {
        if (err) {
          reject(`Error in getGames(): ${err}`)
          done()
          return
        }
        let games = result.rows.map((row) => {
          return {
            id: row.id,
            createdAt: row.created_at,
            verified: row.verified,
            character: {
              id: row.character_id,
              name: row.character_name
            },
            user: {
              id: row.user_id,
              name: row.user_name,
              username: row.user_username,
              character: {
                id: row.user_character_id,
                name: row.user_character_name
              }
            }
          }
        })
        resolve(games)
        done()
      })
    })
  })
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
