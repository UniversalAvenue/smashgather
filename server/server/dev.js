import express from "express"
import graphQLHTTP from "express-graphql"
import path from "path"
import webpack from "webpack"
import config from "../webpack.config"
import WebpackDevServer from "webpack-dev-server"
import {Schema} from "./data/schema"
import expressJwt from "express-jwt"
import jwt from "jsonwebtoken"
import multer from 'multer'

const APP_PORT = 3000
const GRAPHQL_PORT = 8080
const JWT_SECRET = process.env.JWT_SECRET || "MyDevelopmentServerSecret"

// Expose a GraphQL endpoint
var graphQLServer = express()
graphQLServer.use(expressJwt({ secret: JWT_SECRET, credentialsRequired: false }))
graphQLServer.use("/graphql", multer({ storage: multer.memoryStorage() }).single('screenshot'))
graphQLServer.use("/graphql", graphQLHTTP( request => ({
  graphiql: true,
  pretty: true,
  schema: Schema,
  rootValue: {
    user: request.user,
    request: request
  },
})))
graphQLServer.listen(GRAPHQL_PORT, () => console.log(
  `GraphQL Server is now running on http://localhost:${GRAPHQL_PORT}`
))

// Serve the Relay app
var compiler = webpack(config)
var app = new WebpackDevServer(compiler, {
  contentBase: "/public/",
  hot: true,
  proxy: { "/graphql": `http://localhost:${GRAPHQL_PORT}` },
  publicPath: "/assets/",
  stats: { colors: true }
})
// Serve static resources
app.use("/", express.static(path.resolve(__dirname, "../public")))
app.listen(APP_PORT, () => {
  console.log(`App is now running on http://localhost:${APP_PORT}`)

  // Log a valid JWT, so that anyone who has access to server logs can execute mutations :)
  console.log("AUTH TOKEN: ", jwt.sign({ user: "smashgather" }, JWT_SECRET))
})
