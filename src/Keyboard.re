[@react.component]
let make = (~children) => {
  let (_, dispatch) = Store.use();
  let onKeyDown = event => {
    switch (ReactEvent.Keyboard.key(event)->Key.fromString) {
    | Some(key) => dispatch(Store.KeyPressed(key))
    | None => ()
    };
  };
  <div className="keyboard" onKeyDown tabIndex=0> children </div>;
};