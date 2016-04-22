import express from "express"
import compression from "compression"
import graphQLHTTP from "express-graphql"
import path from "path"
import {Schema} from "./data/schema"
import expressJwt from "express-jwt"
import jwt from "jsonwebtoken"
import multer from 'multer'

const JWT_SECRET = process.env.JWT_SECRET
if (!JWT_SECRET) {
  throw new Error("JWT_SECRET environment variable undefined!")
}

let app = express()

app.set("port", (process.env.PORT || 3000))

// Expose a GraphQL endpoint
app.use(expressJwt({ secret: JWT_SECRET, credentialsRequired: false }))
app.use(compression())
app.use("/graphql", multer({ storage: multer.memoryStorage() }).single('screenshot'))
app.use("/graphql", graphQLHTTP( request => ({
  graphiql: true,
  pretty: true,
  schema: Schema,
  rootValue: {
    user: request.user,
    request: request
  },
})))

// Serve static resources
app.use("/", express.static(path.resolve(__dirname, "../public")))
app.listen(app.get("port"), () => {
  console.log("Smashgather App is now running on port", app.get("port"))

  // Log a valid JWT, so that anyone who has access to server logs can execute mutations :)
  console.log("AUTH TOKEN: ", jwt.sign({ user: "smashgather" }, JWT_SECRET))
});
