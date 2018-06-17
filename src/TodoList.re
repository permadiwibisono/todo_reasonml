type todo = {
  id: int,
  title: string,
  completed: bool,
  /* completedAt: option(string) */
};

type state = {
  todos: list(todo)
};

type action = 
  | AddTodo(string)
  | Delete(int)
  | Toggle(int, bool)
;


let checkedFromEvent = (evt) : bool  => (
  evt
  |> ReactEventRe.Form.target
  |> ReactDOMRe.domElementToObj
)##checked;

let lastId = ref(0)

let component = ReasonReact.reducerComponent("TodoList");

let addTodo = (newTodo: string): todo => {
  lastId := lastId^ + 1;
  { id: lastId^, title: newTodo, completed: false }
}

let findAndDeleteTodo = (listOfTodo: list(todo), id: int) : list(todo) => List.filter((todo) => todo.id !== id, listOfTodo)

let findAndToggleTodo = (listOfTodo: list(todo), id: int, completed: bool) : list(todo) => 
  List.map((todo) => {
    if(todo.id===id){
      { ...todo, completed }
    }
    else{
      todo;
    }    
  }, listOfTodo)

module TodoItem {
  let component = ReasonReact.statelessComponent("TodoItem");
  let make = (~todo, ~onToggle, ~onDelete, _) => {
    ...component,
    render: (_self) =>
      <div className=("item"++(todo.completed?" completed":""))>
        <p> 
          <input 
            _type="checkbox" 
            value="true" 
            checked=todo.completed 
            onChange=((evt)=> onToggle(todo.id, checkedFromEvent(evt)))
          /> 
          <span>
          (
            todo.completed?
            <del>
              (ReasonReact.string(todo.title))
            </del>:ReasonReact.string(todo.title)
          )
          </span>
          <a href="#" title="Delete" onClick=((evt)=>{
            ReactEventRe.Mouse.preventDefault(evt);
            onDelete(todo.id);
          })>
            (ReasonReact.string("X"))
          </a>
        </p>
      </div>
  }
}

let make = (_children) => {
  ...component,
  initialState: () => { todos:[] },
  reducer: (action, state) => switch(action){
    | AddTodo(newTodo) => ReasonReact.Update({todos:[ addTodo(newTodo), ...state.todos ]})
    | Delete(id) => ReasonReact.Update({todos: findAndDeleteTodo(state.todos, id)})
    | Toggle(id, completed) => ReasonReact.Update({todos:findAndToggleTodo(state.todos,id, completed)})
  },
  render: ({ state:{todos}, send}) => {
    <div className="todo-container">
      <p className="todo-container--summary">(ReasonReact.string("Total: "++string_of_int(List.length(todos))))</p>
      <div className="todo-container--form">
        <InputTodo onSubmit=(text=> send(AddTodo(text)))/>
      </div>
      <div className="todo-container--list">
        (
          ReasonReact.array(Array.of_list(
            List.map((todo) => 
              <TodoItem 
                key=string_of_int(todo.id)  
                todo 
                onToggle=((id: int, completed: bool)=> send(Toggle(id, completed)))
                onDelete=((id: int)=> send(Delete(id)))
              />, todos)
          ))
        )
      </div>
    </div>
  }
};