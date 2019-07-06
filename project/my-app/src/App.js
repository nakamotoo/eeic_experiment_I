import React, { Component } from "react";
import Button from "@material-ui/core/Button";
import Slider from "./Slider";
import "./App.css";

class App extends Component {
  state = {
    isConnecting: false
  };

  ps;
  // call = () => {
  //   const exec = require("child_process").exec;
  //   var result = "";
  //   exec(
  //     "./../client2 10.213.8.197 50003 | play -t raw -b 16 -c 1 -e s -r 44100 -",
  //     (error, stdout, stderr) => {
  //       result = stdout;
  //     }
  //   );
  //   this.setState({ isConnecting: true });
  // };
  call = () => {
    const { spawn } = require("child_process");
    this.ps = spawn("./../client", ["10.213.8.197", "50005"]);
    const play = spawn("play", [
      "-t",
      "raw",
      "-b",
      "16",
      "-c",
      "1",
      "-e",
      "s",
      "-r",
      "44100",
      "-"
    ]);
    this.ps.stdout.on("data", data => {
      play.stdin.write(data);
    });
  };

  drop = () => {
    this.ps.stdin.write("a");
  };

  render() {
    const { isConnecting } = this.state;
    return (
      <div className="App">
        <header className="App-header">
          <Slider />
          <p> {isConnecting.toString()}</p>
          <Button
            variant="contained"
            color="primary"
            onClick={() => this.drop()}
          >
            切断
          </Button>
          <Button
            variant="contained"
            color="primary"
            onClick={() => this.call()}
          >
            call
          </Button>
        </header>
      </div>
    );
  }
}

export default App;
