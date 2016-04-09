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
              (SELECT COUNT(*) FROM games WHERE games.user_id = users.id) AS wins,
              (SELECT COUNT(*) FROM games WHERE games.user_id = users.id AND games.created_at > (now() - '6 days'::interval)) AS weekly_wins
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
        let user = Object.assign(new User(), {
          id: row.id,
          name: row.name,
          username: row.username,
          wins: row.wins,
          weeklyWins: row.weekly_wins,
          character: {
            id: row.character_id,
            name: row.character_name
          }
        })
        // Remove null records from the output
        if (!user.character.id) { user.character = null }
        resolve(user)
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
                (SELECT COUNT(*) FROM games WHERE games.user_id = users.id AND games.created_at > (now() - '6 days'::interval)) AS user_weekly_wins,
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
        let game = Object.assign(new Game(), {
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
            weeklyWins: row.user_weekly_wins,
            character: {
              id: row.user_character_id,
              name: row.user_character_name,
              wins: row.user_character_wins
            }
          }
        })
        // Remove null records from the output
        if (!game.user.id) { game.user = null }
        if (!game.character.id) { game.character = null }
        resolve(game)
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

function getUsers({ order }) {
  console.log("getUsers")
  return new Promise((resolve, reject) => {
    pg.connect(databaseUrl, (err, client, done) => {
      let orderArg = null
      if (typeof order === "undefined" || order === null || order === "wins") {
        orderArg = "wins"
      } else if (order === "weeklyWins") {
        orderArg = "weekly_wins"
      } else {
        reject(`Invalid order argument: ${order}`)
        return
      }
      console.log("ORDER BY ", orderArg)
      client.query(
          `SELECT users.id, users.name, users.username, users.character_id, characters.name AS character_name,
              (SELECT COUNT(*) FROM games WHERE games.user_id = users.id) AS wins,
              (SELECT COUNT(*) FROM games WHERE games.user_id = users.id AND games.created_at > (now() - '6 days'::interval)) AS weekly_wins,
              (SELECT COUNT(*) FROM games WHERE games.character_id = users.character_id) AS character_wins
            FROM users
            LEFT JOIN characters ON users.character_id = characters.id
            ORDER BY ${orderArg} DESC;`,
          (err, result) => {
        done()
        if (err) {
          reject(`Error in getUsers(): ${err}`)
          return
        }
        let users = result.rows.map((row) => {
          let user = Object.assign(new User(), {
            id: row.id,
            name: row.name,
            username: row.username,
            wins: row.wins,
            weeklyWins: row.weekly_wins,
            character: {
              id: row.character_id,
              name: row.character_name,
              wins: row.character_wins
            }
          })
          // Remove null records from the output
          if (!user.character.id) { user.character = null }
          return user
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
                (SELECT COUNT(*) FROM games WHERE games.user_id = users.id AND games.created_at > (now() - '6 days'::interval)) AS user_weekly_wins,
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
          let game = Object.assign(new Game(), {
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
              weeklyWins: row.user_weekly_wins,
              character: {
                id: row.user_character_id,
                name: row.user_character_name,
                wins: row.user_character_wins
              }
            }
          })
          // Remove null records from the output
          if (!game.user.id) { game.user = null }
          if (!game.character.id) { game.character = null }
          return game
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
      // First, select all users who main that character, sorted by wins
      client.query(
        `SELECT users.id, users.name,
          (SELECT COUNT(*) FROM games WHERE games.user_id = users.id) AS wins
         FROM users
         LEFT JOIN characters ON users.character_id = characters.id
         WHERE characters.name = $1
         ORDER BY wins DESC`
      , [characterName], (err, result) => {
        if (err) {
          reject(`Error in createGame(): ${err}`)
          return
        }
        let insertGameQuery =
          `WITH winning_character AS (
            SELECT id as character_id
            FROM characters
            WHERE name = $1
            LIMIT 1
          ) `
        let insertGameQueryArgs = [characterName]
        if (result.rows.length < 1) {
          console.log(`Found no winning user for character ${characterName}!`)
          insertGameQuery += `INSERT INTO games (character_id, created_at, verified)
          SELECT character_id, CURRENT_TIMESTAMP, FALSE
          FROM winning_character
          RETURNING id;`
        } else {
          console.log(`Selecting ${result.rows[0].name} as winning user for character ${characterName}!`)
          let winningUserId = result.rows[0].id
          insertGameQuery += `INSERT INTO games (character_id, user_id, created_at, verified)
          SELECT character_id, $2, CURRENT_TIMESTAMP, FALSE
          FROM winning_character
          RETURNING id;`
          insertGameQueryArgs.push(winningUserId)
        }
        client.query(insertGameQuery, insertGameQueryArgs, (err, result) => {
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
  })
}

function updateGame({ gameId, userId, characterId, verified }) {
  console.log("updateGame", gameId)
  return new Promise((resolve, reject) => {
    pg.connect(databaseUrl, (err, client, done) => {
      // Query building without an ORM... Fun!
      let changes = ""
      let params = [gameId]
      if (typeof userId != "undefined") {
        params.push(userId)
        changes += `user_id = $${params.length},`
      }
      if (typeof characterId != "undefined") {
        params.push(characterId)
        changes += `character_id = $${params.length},`
      }
      if (typeof verified != "undefined") {
        params.push(verified)
        changes += `verified = $${params.length},`
      }
      if (params.length == 1) {
        // no change!
        resolve(gameId)
        return
      }

      // Remove trailing comma... don't give me that look!
      changes = changes.slice(0, -1)

      client.query(
          `UPDATE games
          SET ${changes}
          WHERE games.id = $1
          RETURNING id;`,
          params,
          (err, result) => {
        done()
        if (err) {
          reject(`Error in updateGame(): ${err}`)
          return
        }
        if (result.rows.length < 1) {
          resolve(null)
          return
        }
        let updatedGameId = result.rows[0].id
        resolve(updatedGameId)
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
  updateGame,
  Character,
  User,
  Game,
};
