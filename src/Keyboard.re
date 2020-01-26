[@react.component]
let make = (~children) => {
  let (state, dispatch) = MutableStore.use();
  let onKey = (keyState, event) => {
    let keys = state^.MutableStore.State.keys;
    switch (ReactEvent.Keyboard.key(event)->Key.fromString) {
    | Some(key) =>
      if (Belt.Option.mapWithDefault(Belt.Map.get(keys, key), true, currentState => currentState != keyState)) {
        dispatch(keyState === Key.Pressed ? MutableStore.KeyPressed(key) : MutableStore.KeyReleased(key));
      }
    | None => ()
    };
  };
  <div className="keyboard" onKeyDown={onKey(Key.Pressed)} onKeyUp={onKey(Key.Released)} tabIndex=0> children </div>;
};