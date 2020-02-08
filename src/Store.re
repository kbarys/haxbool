module State = {
  type t = {gameState: Game.state};
  let initial = {gameState: Game.initState};
};

type action =
  | GameStateChanged(Game.state);

let reducer = (state: State.t, action): State.t => {
  Js.log("reducer called");
  switch (action) {
  | GameStateChanged(gameState) => {gameState: gameState}
  };
};

let reducerWithLogger = (state, action) => {
  let nextState = reducer(state, action);
  Js.log(nextState);
  nextState;
};

let context = React.createContext((State.initial, _ => ()));

module ContextComponent = {
  let makeProps = (~value, ~children, ()) => {
    "value": value,
    "children": children,
  };
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