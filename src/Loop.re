module Performance = {
  [@bs.scope "performance"] [@bs.val] external now: unit => float = "now";
};

let updatePlayerActions = (state: Game.state): Game.state => {
  ...state,
  players:
    Belt.List.map(
      state.players,
      fun
      | {physicalObject: {id}} as player when id == "player_1" => {
          ...player,
          actions: Input.actions^,
        }
      | player => player,
    ),
};

[@bs.scope "window"] [@bs.val]
external requestAnimationFrame: (float => unit) => int =
  "requestAnimationFrame";

let rec loopWithTime =
        (f, previousResult, previousTimestamp, currentTimestamp) => {
  let time = (currentTimestamp -. previousTimestamp) /. 1000.0;
  let result = f(previousResult, ~time);
  let _ = requestAnimationFrame(loopWithTime(f, result, currentTimestamp));
  ();
};

let logTo = (selector, value) => {
  Webapi.Dom.Document.querySelector(selector, Webapi.Dom.document)
  ->Belt.Option.forEach(valueBox => {
      Webapi.Dom.Element.setInnerText(valueBox, value)
    });
};

let logState = (state: Game.state) => {
  let player = Belt.List.getExn(state.players, 0);
  logTo(
    ".force-value",
    player.physicalObject.force->Vector.toStringFixed(~digits=4),
  );
  logTo(
    ".acceleration-value",
    player.physicalObject.acceleration->Vector.toStringFixed(~digits=4),
  );
  logTo(
    ".friction-value",
    PhysicalObject.friction(player.physicalObject)
    ->Vector.toStringFixed(~digits=4),
  );
  logTo(
    ".velocity-value",
    player.physicalObject.velocity->Vector.toStringFixed(~digits=4),
  );
};

let start = canvas => {
  let gameLoop = (previousState, ~time) => {
    let state = Game.nextState(previousState, time)->updatePlayerActions;
    logState(state);
    Scene.render(canvas, previousState, state);
    state;
  };
  Scene.init(canvas);
  loopWithTime(gameLoop, Game.initState, 0.0, 0.0);
};