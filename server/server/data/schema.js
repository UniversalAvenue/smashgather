import {
  GraphQLBoolean,
  GraphQLFloat,
  GraphQLID,
  GraphQLInt,
  GraphQLList,
  GraphQLNonNull,
  GraphQLObjectType,
  GraphQLSchema,
  GraphQLString,
} from "graphql";

import {
  connectionArgs,
  connectionDefinitions,
  connectionFromArray,
  connectionFromPromisedArray,
  fromGlobalId,
  globalIdField,
  mutationWithClientMutationId,
  nodeDefinitions,
} from "graphql-relay";

import {
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
} from "./database";

var {nodeInterface, nodeField} = nodeDefinitions(
  (globalId) => {
    var {type, id} = fromGlobalId(globalId);
    if (type === "Character") {
      return getCharacter(id);
    } else if (type === "User") {
      return getUser(id);
    } else if (type === "Game") {
      return getGame(id);
    } else {
      return null;
    }
  },
  (obj) => {
    if (obj instanceof Character) {
      return characterType;
    } else if (obj instanceof User)  {
      return userType;
    } else if (obj instanceof Game)  {
      return gameType;
    } else {
      return null;
    }
  }
);

var characterType = new GraphQLObjectType({
  name: "Character",
  description: "A Smash character",
  fields: () => ({
    id: globalIdField("Character"),
    name: {
      type: GraphQLString,
      description: "The name of the character",
    },
    wins: {
      type: GraphQLInt,
      description: "The number of wins for the character",
    },
  }),
  interfaces: [nodeInterface],
});

var userType = new GraphQLObjectType({
  name: "User",
  description: "A Smash user",
  fields: () => ({
    id: globalIdField("User"),
    name: {
      type: GraphQLString,
      description: "The name of the user",
    },
    username: {
      type: GraphQLString,
      description: "The Slack username of the user",
    },
    character: {
      type: characterType,
      description: "The main character played by the user",
    },
    wins: {
      type: GraphQLInt,
      description: "The number of wins for the user",
    },
    weeklyWins: {
      type: GraphQLInt,
      description: "The number of wins for the user in the last week",
    },
  }),
  interfaces: [nodeInterface],
});

var gameType = new GraphQLObjectType({
  name: "Game",
  description: "Results of a Smash game",
  fields: () => ({
    id: globalIdField("Game"),
    createdAt: {
      type: GraphQLString,
      description: "Timestamp (with timezone) the game was recorded",
    },
    user: {
      type: userType,
      description: "The winning user of this game",
    },
    character: {
      type: characterType,
      description: "The winning character of this game",
    },
    verified: {
      type: GraphQLBoolean,
      description: "Whether or not the winning user has been verified",
    }
  }),
  interfaces: [nodeInterface],
});

var viewerType = new GraphQLObjectType({
  name: "Viewer",
  description: "Base type for Smashgather queries",
  fields: () => ({
    characters: {
      type: CharacterConnection,
      description: "Available Smash characters",
      args: connectionArgs,
      resolve: (_, args) => connectionFromPromisedArray(getCharacters(), args),
    },
    users: {
      type: UserConnection,
      description: "Available Smash users",
      args: Object.assign(connectionArgs, {
        order: {
          type: GraphQLString
        },
      }),
      resolve: (_, args) => connectionFromPromisedArray(getUsers(args), args),
    },
    games: {
      type: GameConnection,
      description: "Available Smash games",
      args: connectionArgs,
      resolve: (_, args) => connectionFromPromisedArray(getGames(), args),
    },
  }),
});

var {connectionType: CharacterConnection} =
  connectionDefinitions({name: "Character", nodeType: characterType});

var {connectionType: UserConnection} =
  connectionDefinitions({name: "User", nodeType: userType});

var {connectionType: GameConnection} =
  connectionDefinitions({name: "Game", nodeType: gameType});

var queryType = new GraphQLObjectType({
  name: "Query",
  fields: () => ({
    node: nodeField,
    // Add your own root fields here
    viewer: {
      type: viewerType,
      resolve: () => { return {} },
    },
  }),
});

var CreateGameMutation = mutationWithClientMutationId({
  name: "CreateGame",
  inputFields: {
    characterName: {
      type: new GraphQLNonNull(GraphQLString)
    },
  },
  outputFields: {
    game: {
      type: gameType,
      resolve: (payload) => getGame(payload.gameId)
    },
    viewer: {
      type: viewerType,
      resolve: () => { return {} }
    }
  },
  mutateAndGetPayload: ({ createdAt, characterName }, { rootValue }) => {
    return createGame({ characterName, screenshot: rootValue.request.file }).then((newGameId) => {
      return { gameId: newGameId }
    })
  }
});

var UpdateGameMutation = mutationWithClientMutationId({
  name: "UpdateGame",
  inputFields: {
    gameId: {
      type: new GraphQLNonNull(GraphQLID),
    },
    userId: {
      type: GraphQLID,
    },
    characterId: {
      type: GraphQLID,
    },
    verified: {
      type: GraphQLBoolean,
      description: "Whether or not the winning user has been verified",
    }
  },
  outputFields: {
    game: {
      type: gameType,
      resolve: (payload) => getGame(payload.gameId)
    },
    viewer: {
      type: viewerType,
      resolve: () => { return {} }
    }
  },
  mutateAndGetPayload: ({ gameId, userId, characterId, verified }, { rootValue }) => {
    // Unpack the global ID stuff
    if (typeof gameId != "undefined") {
      let {type, id} = fromGlobalId(gameId)
      if (type != "Game" || !id) {
        throw new Error("Invalid gameId!")
      }
      gameId = id;
    }
    if (typeof userId != "undefined") {
      let {type, id} = fromGlobalId(userId)
      if (type != "User" || !id) {
        throw new Error("Invalid userId!")
      }
      userId = id;
    }
    if (typeof characterId != "undefined") {
      let {type, id} = fromGlobalId(characterId)
      if (type != "Character" || !id) {
        throw new Error("Invalid characterId!")
      }
      characterId = id;
    }
    return updateGame({ gameId, userId, characterId, verified }).then((updatedGameId) => {
      return { gameId: updatedGameId }
    })
  }
});

var mutationType = new GraphQLObjectType({
  name: "Mutation",
  fields: () => ({
    createGame: CreateGameMutation,
    updateGame: UpdateGameMutation
  })
});

export var Schema = new GraphQLSchema({
  query: queryType,
  mutation: mutationType
});
