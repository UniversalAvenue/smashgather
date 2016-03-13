var express = require("express")
var bodyParser = require('body-parser')
var app = express()

app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())
app.use(express.static(__dirname + "/public"))

app.set("port", (process.env.PORT || 5000))

// Browser routes
app.set("views", __dirname + "/views")
app.set("view engine", "ejs")
app.get("/", function(request, response) {
  response.render("pages/index")
})

// API routes
app.post("/game", function(request, response) {
  console.log("received POST /game")
  console.log("request.body", request.body)
  if (!request.body) {
    response.sendStatus(400)
  }
  if (request.body.winner) {
    response.status(200).send("got game with winner = " + request.body.winner);
  } else {
    response.status(200).send("sorry, didn't understand your data")
  }
})

app.listen(app.get("port"), function() {
  console.log("smashboard-server running on port", app.get("port"))
})
