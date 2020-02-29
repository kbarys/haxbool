open Belt;

type actions = {
  moveUp: bool,
  moveDown: bool,
  moveLeft: bool,
  moveRight: bool,
  hit: bool,
};

let noActions = {moveUp: false, moveDown: false, moveLeft: false, moveRight: false, hit: false};

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
        position: (Options.virtualWidth /. 4.0, Options.virtualHeight /. 2.0),
        radius: Options.ballRadius,
      },
      mass: Options.ballMass,
      force: Vector.zero,
      acceleration: Vector.zero,
      velocity: Vector.zero,
    },
  },
};

let updatePlayerForce = (previous: player) => {
  let {actions} = previous;
  let force =
    (
      (actions.moveLeft ? (-1.0) : 0.0) +. (actions.moveRight ? 1.0 : 0.0),
      (actions.moveUp ? (-1.0) : 0.0) +. (actions.moveDown ? 1.0 : 0.0),
    )
    ->Vector.multiplyByScalar(Options.playerForce);
  {...previous, physicalObject: previous.physicalObject->PhysicalObject.setForce(force)};
};

let nextState = (previousState: state, time: float) => {
  let objects = [
    previousState.ball.physicalObject,
    ...previousState.players->List.map(player => updatePlayerForce(player).physicalObject),
  ];
  let updatedObjects = Collision.update(objects, time);
  let players =
    previousState.players
    ->Belt.List.map(player =>
        {
          ...player,
          physicalObject: List.getBy(updatedObjects, object_ => object_.id == player.physicalObject.id)->Option.getExn,
        }
      );
  let ball = {
    physicalObject:
      List.getBy(updatedObjects, object_ => object_.id == previousState.ball.physicalObject.id)->Option.getExn,
  };
  {players, ball};
};