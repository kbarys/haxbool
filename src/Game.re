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
        velocity: Vector.zero,
        frictionCoefficient: 0.05,
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
      velocity: Vector.zero,
      frictionCoefficient: 5.0,
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
  {
    ...previous,
    physicalObject: {
      ...previous.physicalObject,
      force,
    },
  };
};

let nextState = (previousState: state, time: float) => {
  let playersWithUpdatedForces = previousState.players->List.map(updatePlayerForce);
  let objectById =
    [previousState.ball.physicalObject, ...playersWithUpdatedForces->List.map(({physicalObject}) => physicalObject)]
    ->List.map(object_ => (object_.id, object_))
    ->List.toArray
    ->Map.String.fromArray;
  let updatedObjects = Physics.update(objectById, time);
  let players =
    previousState.players
    ->Belt.List.map(player =>
        {...player, physicalObject: Map.String.getExn(updatedObjects, player.physicalObject.id)}
      );
  let ball = {physicalObject: Map.String.getExn(updatedObjects, previousState.ball.physicalObject.id)};
  {players, ball};
};