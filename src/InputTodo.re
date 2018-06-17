type action = | OnChange(string) | ClearInput;
let component = ReasonReact.reducerComponent("InputTodo");


let valueFromEvent = (evt) : string  => (
  evt
  |> ReactEventRe.Form.target
  |> ReactDOMRe.domElementToObj
)##value;

let enterKey:string = "Enter";

let make = (~onSubmit, _children) => {
  ...component,
  initialState: () =>  "",
  reducer: (action, _) => switch (action) {
    | OnChange(newText) => ReasonReact.Update(newText)
    | ClearInput => ReasonReact.Update("")
  },
  render:({state: todo, send}) => 
    <input
      className="todo-container--input-todo"
      value=todo
      _type="text"
      placeholder="Write Something to do!"
      onChange=((evt) => send(OnChange(valueFromEvent(evt))))
      onKeyDown=((evt) =>
        if (ReactEventRe.Keyboard.key(evt) == enterKey) {
          onSubmit(todo);
          send(ClearInput);
        }
      )
    />
}