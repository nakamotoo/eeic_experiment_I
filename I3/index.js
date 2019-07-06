import React from "react";
import { render } from "react-dom";

class MyApp extends React.Component {
  render() {
    return <div>Hello World!</div>;
  }
}
render(<MyApp />, document.getElementById("root"));
