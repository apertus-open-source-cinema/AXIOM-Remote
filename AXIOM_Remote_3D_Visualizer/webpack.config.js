const path = require("path");
const CopyPlugin = require("copy-webpack-plugin");

module.exports = {
  stats: { errorDetails: true },
  entry: ["./src/js/main.js"],
  mode: "development",
  output: {
    path: path.resolve(__dirname, "dist"),
    filename: "js/bundle.js",
    clean: true,
    webassemblyModuleFilename: "js/[hash].wasm",
  },
  target: "web",
  devServer: {
    contentBase: path.join(__dirname, "dist"),
    watchOptions: {
      ignored: /node_modules/,
    },
  },
  plugins: [
    new CopyPlugin({
      patterns: [
        { from: "src/index.html", to: "." },
        {
          context: path.join(__dirname, "src"),
          from: "css/",
          to: "css/",
        },
        {
          context: path.join(__dirname, "src"),
          from: "data/",
          to: "data/",
        },
        {
          context: path.join(__dirname, "src"),
          from: "js/FW/axiom_remote_firmware.wasm",
          to: "js/",
        },
      ],
    }),
  ],
  experiments: {
    asyncWebAssembly: true,
  },
};
