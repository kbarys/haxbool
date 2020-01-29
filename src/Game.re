type actions = {
  moveUp: bool,
  moveDown: bool,
  moveLeft: bool,
  moveRight: bool,
  hit: bool,
};

let noActions = {
  moveUp: false,
  moveDown: false,
  moveLeft: false,
  moveRight: false,
  hit: false,
};

type player = {
  position: Vector.t,
  speed: Vector.t,
  actions,
};

type state = {players: Belt.Map.String.t(player)};

let initState = {
  players:
    Belt.Map.String.fromArray([|
      (
        "1",
        {
          position: (Options.width /. 2.0, Options.height /. 2.0),
          speed: (0.0, 0.0),
          actions: noActions,
        },
      ),
    |]),
};

let nextPlayerSpeed = (progress, player) => {
  let (hSpeed, vSpeed) = player.speed;
  let accelartion = Options.playerAcceleration *. progress;
  let break = Options.playerBreak *. progress;
  let nextSpeed = (previous, moveOpposite, moveForward) =>
    switch (moveOpposite, moveForward) {
    | (false, false)
    | (true, true) => Speed.decrease(previous, ~by=break)
    | (true, false) when previous > 0.0 =>
      Speed.increase(previous, ~by=(-1.2) *. accelartion)
    | (true, false) => Speed.increase(previous, ~by=-. accelartion)
    | (false, true) when previous < 0.0 =>
      Speed.increase(previous, ~by=1.2 *. accelartion)
    | (false, true) => Speed.increase(previous, ~by=accelartion)
    };
  (
    nextSpeed(hSpeed, player.actions.moveLeft, player.actions.moveRight),
    nextSpeed(vSpeed, player.actions.moveUp, player.actions.moveDown),
  );
};

let nextPosition = (progress, position, speed) => {
  position
  ->Vector.add(Vector.multiply(speed, ~by=progress))
  ->Vector.max((Options.playerRadius, Options.playerRadius))
  ->Vector.min((
      Options.width -. Options.playerRadius,
      Options.height -. Options.playerRadius,
    ));
};

let nextPlayerState = (progress, previous) => {
  ...previous,
  position: nextPosition(progress, previous.position, previous.speed),
  speed: nextPlayerSpeed(progress, previous),
};

let nextState = (previousState: state, progress: float) => {
  {
    players:
      previousState.players->Belt.Map.String.map(nextPlayerState(progress)),
  };
};
