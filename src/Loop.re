module Performance = {
  [@bs.scope "performance"] [@bs.val] external now: unit => float = "now";
};

[@bs.scope "window"] [@bs.val] external requestAnimationFrame: (float => unit) => int = "requestAnimationFrame";

let rec loopWithTime = (f, previousResult, previousTimestamp, currentTimestamp) => {
  let time = (currentTimestamp -. previousTimestamp) /. 1000.0;
  let result = f(previousResult, ~time);
  let _ = requestAnimationFrame(loopWithTime(f, result, currentTimestamp));
  ();
};

let logTo = (selector, value) => {
  Webapi.Dom.Document.querySelector(selector, Webapi.Dom.document)
  ->Belt.Option.forEach(valueBox => {Webapi.Dom.Element.setInnerText(valueBox, value)});
};

let logState = (state: Game.state) => {
  let player = Belt.List.getExn(state.players, 0);
  logTo(".force-value", player.physicalObject.force->Vector.toStringFixed(~digits=4));
  logTo(".acceleration-value", PhysicalObject.acceleration(player.physicalObject)->Vector.toStringFixed(~digits=4));
  logTo(".friction-value", PhysicalObject.friction(player.physicalObject)->Vector.toStringFixed(~digits=4));
  logTo(".velocity-value", player.physicalObject.velocity->Vector.toStringFixed(~digits=4));
  logTo(".hit", player.actions.hit ? "True" : "False");
};

let start = canvas => {
  let gameLoop = (state, ~time) => {
    logState(state);
    let nextState = Game.nextState(state, time);
    Scene.render(canvas, nextState, state);
    nextState;
  };
  Scene.init(canvas);
  loopWithTime(gameLoop, Game.initState, 0.0, 0.0);
};