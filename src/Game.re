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
  players: Belt.List.t(player),
  ball,
};

let initState = {
  players: [
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
  ],
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
};

let nextPlayerState = (previous: player, time) => {
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

let nextBallState = (previous: ball, time) => {
  {
    physicalObject:
      previous.physicalObject
      ->PhysicalObject.updatePosition(time)
      ->PhysicalObject.updateVelocity(time)
      ->PhysicalObject.updateAcceleration,
  };
};

let nextState = (previousState: state, time: float) => {
  let objects = [
    previousState.ball.physicalObject,
    ...previousState.players->Belt.List.map(player => player.physicalObject),
  ];
  let objectsAfterCollisionsById =
    Collision.simulateElasticCollisions(objects)
    ->Belt.List.map(object_ => (object_.id, object_))
    ->Belt.List.toArray
    ->Belt.Map.String.fromArray;
  let players =
    previousState.players
    ->Belt.List.map(player =>
        {
          ...player,
          physicalObject:
            Belt.Map.String.getWithDefault(
              objectsAfterCollisionsById,
              player.physicalObject.id,
              player.physicalObject,
            ),
        }
      );
  let ball = {
    physicalObject:
      Belt.Map.String.getWithDefault(
        objectsAfterCollisionsById,
        previousState.ball.physicalObject.id,
        previousState.ball.physicalObject,
      ),
  };
  {
    players: Belt.List.map(players, nextPlayerState(_, time)),
    ball: nextBallState(ball, time),
  };
};