var path = require("path")

module.exports = {
  bail: true,
  entry: path.resolve(__dirname, "js", "app.js"),
  module: {
    loaders: [
      {
        exclude: /node_modules/,
        loader: "babel",
        test: /\.js$/,
      }
    ]
  },
  output: {
    path: path.join(__dirname, "public/assets"),
    filename: "app.js",
    publicPath: "/assets/"
  }
};
