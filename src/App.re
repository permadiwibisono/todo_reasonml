[%bs.raw {|require('./App.css')|}];

let component = ReasonReact.statelessComponent("App");

let make = (~message, _children) => {
  ...component,
  render: _self =>
    <div className="App">
      <Header title=message/>
      <TodoList/>
    </div>,
};
