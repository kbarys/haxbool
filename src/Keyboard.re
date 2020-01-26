[@react.component]
let make = (~children) => {
  let ({Store.State.keys}, dispatch) = Store.use();
  let onKey = (state, event) =>
    switch (ReactEvent.Keyboard.key(event)->Key.fromString) {
    | Some(key) =>
      if (Belt.Option.mapWithDefault(Belt.Map.get(keys, key), true, currentState => currentState != state)) {
        dispatch(state === Key.Pressed ? Store.KeyPressed(key) : Store.KeyReleased(key));
      }
    | None => ()
    };
  <div className="keyboard" onKeyDown={onKey(Key.Pressed)} onKeyUp={onKey(Key.Released)} tabIndex=0> children </div>;
};