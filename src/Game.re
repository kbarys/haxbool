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
  actions,
  physicalObject: PhysicalObject.t,
};

type ball = {physicalObject: PhysicalObject.t};

type state = {
  players: Belt.Map.String.t(player),
  ball,
  collisionPoint: option(Vector.t),
};

let initState = {
  players:
    Belt.Map.String.fromArray([|
      (
        "1",
        {
          physicalObject: {
            id: "player_1",
            circle: {
              position: (0.25, 0.5),
              radius: Options.playerRadius,
            },
            mass: Options.playerMass,
            force: Vector.zero,
            acceleration: Vector.zero,
            velocity: Vector.zero,
          },
          actions: noActions,
        },
      ),
    |]),
  ball: {
    physicalObject: {
      id: "ball",
      circle: {
        position: (Options.virtualWidth /. 2.0, Options.virtualHeight /. 2.0),
        radius: Options.ballRadius,
      },
      mass: Options.ballMass,
      force: Vector.zero,
      acceleration: Vector.zero,
      velocity: Vector.zero,
    },
  },
  collisionPoint: None,
};

let nextState = (previousState: state, time: float) => {
  let nextPlayerState = (previous: player) => {
    let {actions} = previous;
    let force =
      (
        (actions.moveLeft ? (-1.0) : 0.0) +. (actions.moveRight ? 1.0 : 0.0),
        (actions.moveUp ? (-1.0) : 0.0) +. (actions.moveDown ? 1.0 : 0.0),
      )
      ->Vector.multiplyByScalar(Options.playerForce);
    {
      ...previous,
      physicalObject:
        previous.physicalObject
        ->PhysicalObject.setForce(force)
        ->PhysicalObject.updatePosition(time)
        ->PhysicalObject.updateVelocity(time)
        ->PhysicalObject.updateAcceleration,
    };
  };
  let collisions =
    Collision.findCollisions([
      previousState.ball.physicalObject,
      ...Belt.Map.String.valuesToArray(previousState.players)
         ->Belt.List.fromArray
         ->Belt.List.map(x => x.physicalObject),
    ]);
  let collisionPoint =
    switch (collisions) {
    | [c] => Some(c.point)
    | _ => None
    };
  {
    players: previousState.players->Belt.Map.String.map(nextPlayerState),
    ball: previousState.ball,
    collisionPoint,
  };
};