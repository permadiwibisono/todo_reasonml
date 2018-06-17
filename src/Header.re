[@bs.module] external logo : string = "./logo.svg";

let component = ReasonReact.statelessComponent("Header");

let make = (~title, _)=> {
  ...component,
  render: _self =>{
    <div className="App--header">
      <p><img src=logo/> (ReasonReact.string(title))</p>
    </div>
  }
}