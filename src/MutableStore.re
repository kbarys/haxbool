module State = {
  type options = {
    size: float,
    width: float,
    height: float,
    maxPlayerSpeed: float,
    playerAcceleration: float,
    playerBreak: float,
  };
  type player = {
    speed: (float, float),
    position: (float, float),
  };
  type t = {
    keys: Belt.Map.t(Key.t, Key.state, Key.Comparator.identity),
    options,
    player,
    scene: option(Webapi.Dom.Element.t),
  };
  let initial = {
    let size = 1000.0;
    let width = size;
    let height = size /. 2.0;
    {
      keys: Belt.Map.make(~id=(module Key.Comparator)),
      options: {
        size,
        width,
        height,
        maxPlayerSpeed: 0.003 *. size,
        playerAcceleration: 0.0001 *. size,
        playerBreak: 0.00001 *. size,
      },
      player: {
        speed: (0.0, 0.0),
        position: (width *. 0.5, height *. 0.5),
      },
      scene: None,
    };
  };
  let current = ref(initial);
};

type action =
  | KeyPressed(Key.t)
  | KeyReleased(Key.t)
  | FocusLost
  | PlayerSpeedChanged((float, float))
  | PlayerPositionChanged((float, float))
  | SceneLoaded(Webapi.Dom.Element.t);

let reducer = (state: State.t, action): State.t => {
  switch (action) {
  | KeyPressed(key) => {...state, keys: Belt.Map.update(state.keys, key, _ => Some(Pressed))}
  | KeyReleased(key) => {...state, keys: Belt.Map.update(state.keys, key, _ => Some(Released))}
  | FocusLost => {...state, keys: Belt.Map.map(state.keys, _ => Key.Released)}
  | PlayerSpeedChanged(speed) => {
      ...state,
      player: {
        ...state.player,
        speed,
      },
    }
  | PlayerPositionChanged(position) => {
      ...state,
      player: {
        ...state.player,
        position,
      },
    }
  | SceneLoaded(scene) => {...state, scene: Some(scene)}
  };
};

let reducerWithLogger = (state, action) => {
  let nextState = reducer(state, action);
  Js.log(nextState);
  nextState;
};

let dispatch = action => {
  let nextState = reducer(State.current^, action);
  State.current := nextState;
};

let use = () => (State.current, dispatch);