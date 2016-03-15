var express = require("express")
var graphQLHTTP = require("express-graphql")
var path = require("path")
var Schema = require("./data/schema");
var app = express();

app.set("port", (process.env.PORT || 3000))

// Expose a GraphQL endpoint
app.use("/graphql", graphQLHTTP({
  graphiql: true,
  pretty: true,
  schema: Schema,
}))

// Serve static resources
app.use("/", express.static(path.resolve(__dirname, "public")))
app.listen(app.get("port"), () => {
  console.log("Smashgather App is now running on port", app.get("port"))
});
