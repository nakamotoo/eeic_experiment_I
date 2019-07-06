import React, { Component } from "react";
import Button from "@material-ui/core/Button";
import { makeStyles } from "@material-ui/core/styles";
import Slider from "@material-ui/lab/Slider";
import Fab from "@material-ui/core/Fab";
import AddIcon from "@material-ui/icons/Call";
import "./App.css";

function valuetext(value) {
  return `${value}dB`;
}

const marks = [
  {
    value: 0,
    label: "0dB"
  },
  {
    value: 1,
    label: "10dB"
  },
  {
    value: 2,
    label: "20dB"
  }
];

class App extends Component {
  state = {
    isConnecting: false,
    val1: 1,
    val2: 1,
    val3: 1
  };

  ps;

  call = () => {
    this.setState({ isConnecting: true });
    const { spawn } = require("child_process");
    this.ps = spawn("./../client_n", ["10.213.8.197", "50000"]);
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
    this.setState({ isConnecting: false });
    this.ps.stdin.write("a");
  };

  onChangeVal1 = (e, v) => {
    this.setState({ val1: v });
    switch (v) {
      case 0:
        // this.ps.stdin.write("a");
        break;
      case 1:
        // this.ps.stdin.write("a");
        break;
      case 2:
        // this.ps.stdin.write("a");
        break;
    }
  };

  onChangeVal2 = (e, v) => {
    this.setState({ val2: v });
    switch (v) {
      case 0:
        // this.ps.stdin.write("a");
        break;
      case 1:
        // this.ps.stdin.write("a");
        break;
      case 2:
        // this.ps.stdin.write("a");
        break;
    }
  };

  onChangeVal3 = (e, v) => {
    this.setState({ val3: v });
    switch (v) {
      case 0:
        // this.ps.stdin.write("a");
        break;
      case 1:
        // this.ps.stdin.write("a");
        break;
      case 2:
        // this.ps.stdin.write("a");
        break;
    }
  };

  renderButton = () => {
    if (this.state.isConnecting) {
      return (
        <Button
          variant="contained"
          color="secondary"
          onClick={() => this.drop()}
        >
          切断
        </Button>
      );
    } else {
      return (
        <Button variant="contained" color="primary" onClick={() => this.call()}>
          call
        </Button>
      );
    }
  };

  renderSlider = () => {
    return (
      <div className={"slider-wrapper"}>
        <p>
          {this.state.val1} : {this.state.val2} : {this.state.val3}
        </p>

        <Slider
          className={"slider"}
          orientation="vertical"
          getAriaValueText={valuetext}
          defaultValue={1}
          onChange={(e, v) => this.onChangeVal1(e, v)}
          aria-labelledby="vertical-slider"
          min={0}
          max={2}
          step={1}
        />
        <Slider
          className={"slider"}
          orientation="vertical"
          getAriaValueText={valuetext}
          defaultValue={1}
          onChange={(e, v) => this.onChangeVal2(e, v)}
          aria-labelledby="vertical-slider"
          min={0}
          max={2}
          step={1}
        />
        <Slider
          className={"slider"}
          orientation="vertical"
          defaultValue={1}
          onChange={(e, v) => this.onChangeVal3(e, v)}
          aria-labelledby="vertical-slider"
          getAriaValueText={valuetext}
          marks={marks}
          min={0}
          max={2}
          step={1}
        />
      </div>
    );
  };

  render() {
    const { isConnecting } = this.state;
    return (
      <div className="App">
        <header className="App-header">
          {this.renderSlider()}
          <p> {isConnecting.toString()}</p>
          {this.renderButton()}
          <Button
            variant="contained"
            color="secondary"
            onClick={() => this.drop()}
          >
            切断
          </Button>
          <Fab size="small" color="secondary" aria-label="Add">
            <AddIcon />
          </Fab>
        </header>
      </div>
    );
  }
}

export default App;
