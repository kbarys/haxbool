module Performance = {
  [@bs.scope "performance"] [@bs.val] external now: unit => float = "now";
};

let updatePlayerActions = (state: Game.state): Game.state => {
  Game.players:
    Belt.Map.String.update(
      state.players,
      "1",
      Belt.Option.map(_, player => {...player, actions: Input.actions^}),
    ),
};

[@bs.scope "window"] [@bs.val]
external requestAnimationFrame: (float => unit) => int =
  "requestAnimationFrame";

let rec loop =
        (currentTimestamp, ~previousTimestamp, canvasElement, previousState) => {
  let progress = currentTimestamp -. previousTimestamp;
  let state = Game.nextState(previousState, progress)->updatePlayerActions;
  Scene.render(canvasElement, state);
  let _ =
    requestAnimationFrame(
      loop(_, ~previousTimestamp=currentTimestamp, canvasElement, state),
    );
  ();
};
