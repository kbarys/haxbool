module State = {
  type t = {keys: Belt.Map.t(Key.t, Key.state, Key.Comparator.identity)};
  let initial = {keys: Belt.Map.make(~id=(module Key.Comparator))};
};

type action =
  | KeyPressed(Key.t)
  | KeyReleased(Key.t);

let reducer = (state: State.t, action): State.t => {
  switch (action) {
  | KeyPressed(key) => {keys: Belt.Map.update(state.keys, key, _ => Some(Pressed))}
  | KeyReleased(key) => {keys: Belt.Map.update(state.keys, key, _ => Some(Released))}
  };
};

let reducerWithLogger = (state, action) => {
  let nextState = reducer(state, action);
  Js.log(nextState);
  nextState;
};

let context = React.createContext((State.initial, _ => ()));

module ContextComponent = {
  let makeProps = (~value, ~children, ()) => {"value": value, "children": children};
  let make = React.Context.provider(context);
};

module Provider = {
  [@react.component]
  let make = (~children) => {
    let store = React.useReducer(reducerWithLogger, State.initial);
    <ContextComponent value=store> children </ContextComponent>;
  };
};

let use = () => React.useContext(context);