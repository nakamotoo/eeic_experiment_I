import React, { Component } from "react";
import Button from "@material-ui/core/Button";
import { makeStyles } from "@material-ui/core/styles";
import Slider from "@material-ui/lab/Slider";
import Fab from "@material-ui/core/Fab";
import CallIcon from "@material-ui/icons/Call";
import CallEndIcon from "@material-ui/icons/CallEnd";
import TextField from "@material-ui/core/TextField";
import "./App.css";

function valuetext(value) {
  return `${value}dB`;
}

const marks = [
  {
    value: 0,
    label: "-20dB"
  },
  {
    value: 1,
    label: "0dB"
  },
  {
    value: 2,
    label: "6dB"
  }
];

class App extends Component {
  state = {
    isConnecting: false,
    ip: "157.82.202.5",
    port: "50000",
    val1: 1,
    val2: 1,
    val3: 1
  };

  ps;

  call = () => {
    const { ip, port } = this.state;
    this.setState({ isConnecting: true });
    const { spawn } = require("child_process");
    // this.ps = spawn("./../client_n", ["10.213.8.197", "50000"]);
    this.ps = spawn("./../client", [ip, port]);
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

  onChangeIP = (e, v) => {
    this.setState({ ip: v });
  };

  onChangePort = (e, v) => {
    this.setState({ port: v });
  };

  onChangeVal1 = (e, v) => {
    this.setState({ val1: v });
    switch (v) {
      case 0:
        this.ps.stdin.write("b");
        break;
      case 1:
        this.ps.stdin.write("c");
        break;
      case 2:
        this.ps.stdin.write("d");
        break;
    }
  };

  onChangeVal2 = (e, v) => {
    this.setState({ val2: v });
    switch (v) {
      case 0:
        this.ps.stdin.write("e");
        break;
      case 1:
        this.ps.stdin.write("f");
        break;
      case 2:
        this.ps.stdin.write("g");
        break;
    }
  };

  onChangeVal3 = (e, v) => {
    this.setState({ val3: v });
    switch (v) {
      case 0:
        this.ps.stdin.write("h");
        break;
      case 1:
        this.ps.stdin.write("i");
        break;
      case 2:
        this.ps.stdin.write("j");
        break;
    }
  };

  renderButton = () => {
    if (this.state.isConnecting) {
      return (
        // <Button
        //   variant="contained"
        //   color="secondary"
        //   onClick={() => this.drop()}
        // >
        //   切断
        // </Button>
        <Fab color="secondary" aria-label="call" onClick={() => this.drop()}>
          <CallEndIcon />
        </Fab>
      );
    } else {
      return (
        // <Button variant="contained" color="primary" onClick={() => this.call()}>
        //   call
        // </Button>
        <Fab color="primary" aria-label="call" onClick={() => this.call()}>
          <CallIcon />
        </Fab>
      );
    }
  };

  renderSlider = () => {
    return (
      <div className={"slider-wrapper"}>
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
          <TextField
            id="standard-name"
            label="IP Adderess"
            value={this.state.ip}
            onChange={(e, v) => this.onChangeIP(v)}
            margin="normal"
          />
          <TextField
            id="standard-name"
            label="Port Number"
            value={this.state.port}
            onChange={(e, v) => this.onChangePort(v)}
            margin="normal"
          />
          {this.renderButton()}
        </header>
      </div>
    );
  }
}

export default App;
