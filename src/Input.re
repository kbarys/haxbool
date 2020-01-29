let actions =
  ref(
    Game.{
      moveUp: false,
      moveDown: false,
      moveLeft: false,
      moveRight: false,
      hit: false,
    },
  );
let onChange = (key, pressed) => {
  switch (key) {
  | "ArrowUp" => actions := {...actions^, moveUp: pressed}
  | "ArrowDown" => actions := {...actions^, moveDown: pressed}
  | "ArrowLeft" => actions := {...actions^, moveLeft: pressed}
  | "ArrowRight" => actions := {...actions^, moveRight: pressed}
  | " " => actions := {...actions^, hit: pressed}
  | _ => ()
  };
};
let onKeyDown = event =>
  event->Webapi.Dom.KeyboardEvent.key->onChange(_, true);
let onKeyUp = event =>
  event->Webapi.Dom.KeyboardEvent.key->onChange(_, false);
let track = () => {
  Webapi.Dom.Document.addKeyDownEventListener(onKeyDown, Webapi.Dom.document);
  Webapi.Dom.Document.addKeyUpEventListener(onKeyUp, Webapi.Dom.document);
};