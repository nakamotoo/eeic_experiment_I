import React from "react";
import { makeStyles } from "@material-ui/core/styles";
import Slider from "@material-ui/lab/Slider";

import "./App.css";

const useStyles = makeStyles({
  root: {
    height: 100
  }
});

function valuetext(value) {
  return `${value}dB`;
}

const marks = [
  {
    value: 0,
    label: "0dB"
  },
  {
    value: 10,
    label: "10dB"
  },
  {
    value: 20,
    label: "20dB"
  }
];

export default function VerticalSlider() {
  const classes = useStyles();

  return (
    <div className={"Slider"}>
      <Slider
        orientation="vertical"
        getAriaValueText={valuetext}
        defaultValue={10}
        aria-labelledby="vertical-slider"
        min={0}
        max={20}
      />
      <Slider
        orientation="vertical"
        getAriaValueText={valuetext}
        defaultValue={10}
        aria-labelledby="vertical-slider"
        min={0}
        max={20}
      />
      <Slider
        orientation="vertical"
        defaultValue={10}
        aria-labelledby="vertical-slider"
        getAriaValueText={valuetext}
        marks={marks}
        min={0}
        max={20}
      />
    </div>
  );
}
