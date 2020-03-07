type playerActions = {
  moveUp: bool,
  moveDown: bool,
  moveLeft: bool,
  moveRight: bool,
  hit: bool,
};

type t = {
  physicalObject: PhysicalObject.t,
  actions: playerActions,
};

let create = (~id, ~position) => {
  {
    physicalObject: {
      id,
      circle: {
        position,
        radius: Options.playerRadius,
      },
      mass: Options.playerMass,
      force: Vector.zero,
      velocity: Vector.zero,
      frictionCoefficient: Options.playerFrictionCoefficient,
    },
    actions: {
      moveUp: false,
      moveDown: false,
      moveLeft: false,
      moveRight: false,
      hit: false,
    },
  };
};

let updateForce = player => {
  let {actions} = player;
  let force =
    (
      (actions.moveLeft ? (-1.0) : 0.0) +. (actions.moveRight ? 1.0 : 0.0),
      (actions.moveUp ? (-1.0) : 0.0) +. (actions.moveDown ? 1.0 : 0.0),
    )
    ->Vector.multiplyByScalar(Options.playerForce);
  {
    ...player,
    physicalObject: {
      ...player.physicalObject,
      force,
    },
  };
};