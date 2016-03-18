var path = require("path");
var webpack = require("webpack");

module.exports = {
  entry: {
    "app": [
      'webpack-dev-server/client?http://localhost:3000',
      'webpack/hot/only-dev-server',
      "./js/app"
    ]
  },
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
  },
  plugins: [
    new webpack.HotModuleReplacementPlugin()
  ]
};
