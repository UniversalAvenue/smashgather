var pg = require("pg")
var databaseUrl = process.env.DATABASE_URL || "pg://localhost/smashgather"

// GraphQL object types
class Character {}
class User {}
class Game {}

function getCharacter(id) {
  console.log("getCharacter", id)
  return new Promise((resolve, reject) => {
    pg.connect(databaseUrl, (err, client, done) => {
      client.query(
          `SELECT characters.*,
            (SELECT COUNT(*) FROM games WHERE games.character_id = characters.id) AS wins
          FROM characters WHERE id = $1`,
          [id], (err, result) => {
        done()
        if (err) {
          reject(`Error in getCharacter(): ${err}`)
          return
        }
        if (result.rows.length < 1) {
          resolve(null)
          return
        }
        let row = result.rows[0]
        resolve(Object.assign(new Character(), {
          id: row.id,
          name: row.name,
        }))
      })
    })
  })
}

function getUser(id) {
  console.log("getUser", id)
  return new Promise((resolve, reject) => {
    pg.connect(databaseUrl, (err, client, done) => {
      client.query(
          `SELECT users.id, users.name, users.username, users.character_id, characters.name AS character_name,
              (SELECT COUNT(*) FROM games WHERE games.user_id = users.id) AS wins
            FROM users
            LEFT JOIN characters ON users.character_id = characters.id
            WHERE users.id = $1;`,
          [id],
          (err, result) => {
        done()
        if (err) {
          reject(`Error in getUser(): ${err}`)
          return
        }
        if (result.rows.length < 1) {
          resolve(null)
          return
        }
        let row = result.rows[0]
        resolve(Object.assign(new User(), {
          id: row.id,
          name: row.name,
          username: row.username,
          wins: row.wins,
          character: {
            id: row.character_id,
            name: row.character_name
          }
        }))
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
                (SELECT COUNT(*) FROM games WHERE games.user_id = users.id) AS user_wins,
              user_characters.name AS user_character_name,
                (SELECT COUNT(*) FROM games WHERE games.character_id = user_characters.id) AS user_character_wins,
              characters.name AS character_name,
                (SELECT COUNT(*) FROM games WHERE games.character_id = characters.id) AS character_wins
            FROM games
            LEFT JOIN users ON games.user_id = users.id
            LEFT JOIN characters AS user_characters on users.character_id = user_characters.id
            LEFT JOIN characters on games.character_id = characters.id
            WHERE games.id = $1;`,
          [id],
          (err, result) => {
        done()
        if (err) {
          reject(`Error in getGame(): ${err}`)
          return
        }
        if (result.rows.length < 1) {
          resolve(null)
          return
        }
        let row = result.rows[0]
        resolve(Object.assign(new Game(), {
          id: row.id,
          createdAt: row.created_at,
          verified: row.verified,
          character: {
            id: row.character_id,
            name: row.character_name,
            wins: row.character_wins
          },
          user: {
            id: row.user_id,
            name: row.user_name,
            username: row.user_username,
            wins: row.user_wins,
            character: {
              id: row.user_character_id,
              name: row.user_character_name,
              wins: row.user_character_wins
            }
          }
        }))
      })
    })
  })
}

function getCharacters() {
  console.log("getCharacters")
  return new Promise((resolve, reject) => {
    pg.connect(databaseUrl, (err, client, done) => {
      client.query(
          `SELECT characters.*,
            (SELECT COUNT(*) FROM games WHERE games.character_id = characters.id) AS wins
          FROM characters
          ORDER BY wins DESC;`,
          (err, result) => {
        done()
        if (err) {
          reject(`Error in getCharacters(): ${err}`)
          return
        }
        let characters = result.rows.map((row) => {
          return {
            id: row.id,
            name: row.name,
            wins: row.wins
          }
        })
        resolve(characters)
      })
    })
  })
}

function getUsers() {
  console.log("getUsers")
  return new Promise((resolve, reject) => {
    pg.connect(databaseUrl, (err, client, done) => {
      client.query(
          `SELECT users.id, users.name, users.username, users.character_id, characters.name AS character_name,
              (SELECT COUNT(*) FROM games WHERE games.user_id = users.id) AS wins,
              (SELECT COUNT(*) FROM games WHERE games.character_id = users.character_id) AS character_wins
            FROM users
            LEFT JOIN characters ON users.character_id = characters.id
            ORDER BY wins DESC;`,
          (err, result) => {
        done()
        if (err) {
          reject(`Error in getUsers(): ${err}`)
          return
        }
        let users = result.rows.map((row) => {
          return {
            id: row.id,
            name: row.name,
            username: row.username,
            wins: row.wins,
            character: {
              id: row.character_id,
              name: row.character_name,
              wins: row.character_wins
            }
          }
        })
        resolve(users)
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
                (SELECT COUNT(*) FROM games WHERE games.user_id = users.id) AS user_wins,
              user_characters.name AS user_character_name,
                (SELECT COUNT(*) FROM games WHERE games.character_id = user_characters.id) AS user_character_wins,
              characters.name AS character_name,
                (SELECT COUNT(*) FROM games WHERE games.character_id = characters.id) AS character_wins
            FROM games
            LEFT JOIN users ON games.user_id = users.id
            LEFT JOIN characters AS user_characters on users.character_id = user_characters.id
            LEFT JOIN characters on games.character_id = characters.id
            ORDER BY created_at DESC;`,
          (err, result) => {
        done()
        if (err) {
          reject(`Error in getGames(): ${err}`)
          return
        }
        let games = result.rows.map((row) => {
          return {
            id: row.id,
            createdAt: row.created_at,
            verified: row.verified,
            character: {
              id: row.character_id,
              name: row.character_name,
              wins: row.character_wins
            },
            user: {
              id: row.user_id,
              name: row.user_name,
              username: row.user_username,
              wins: row.user_wins,
              character: {
                id: row.user_character_id,
                name: row.user_character_name,
                wins: row.user_character_wins
              }
            }
          }
        })
        resolve(games)
      })
    })
  })
}

function createGame({ characterName }) {
  console.log(`createGame, characterName: ${characterName}`)
  return new Promise((resolve, reject) => {
    pg.connect(databaseUrl, (err, client, done) => {
      client.query(
        `WITH winning_character AS (
          SELECT id as character_id
          FROM characters
          WHERE name = $1
          LIMIT 1
        ), winning_user AS (
          SELECT users.id as user_id
          FROM users
          LEFT JOIN characters ON users.character_id = characters.id
          WHERE characters.name = $1
          LIMIT 1
        )
        INSERT INTO games (character_id, user_id, created_at, verified)
        SELECT character_id, user_id, CURRENT_TIMESTAMP, FALSE
        FROM winning_character, winning_user
        RETURNING id;`
      , [characterName], (err, result) => {
        done()
        if (err) {
          reject(`Error in createGame(): ${err}`)
          return
        }
        if (result.rows.length < 1) {
          resolve(null)
          return
        }
        let newGameId = result.rows[0].id
        resolve(newGameId)
      })
    })
  })
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
