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

type ball = {
  circle: Circle.t,
  speed: Vector.t,
};

type state = {
  players: Belt.Map.String.t(player),
  ball,
};

let initState = {
  players:
    Belt.Map.String.fromArray([|
      (
        "1",
        {
          physicalObject: {
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
    circle: {
      position: (Options.width /. 2.0, Options.height /. 2.0),
      radius: Options.ballRadius,
    },
    speed: (0.0, 0.0),
  },
};

let nextState = (previousState: state, time: float) => {
  let nextPlayerState = (previous: player) => {
    let {actions} = previous;
    let force =
      (
        (actions.moveLeft ? (-1.0) : 0.0) +. (actions.moveRight ? 1.0 : 0.0),
        (actions.moveUp ? (-1.0) : 0.0) +. (actions.moveDown ? 1.0 : 0.0),
      )
      ->Vector.multiplyByScalar(20.0);
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
  {
    players: previousState.players->Belt.Map.String.map(nextPlayerState),
    ball: previousState.ball,
  };
};