type playerActions = {
  moveUp: bool,
  moveDown: bool,
  moveLeft: bool,
  moveRight: bool,
  hit: bool,
};

type t = {
  id: string,
  physicalObject: PhysicalObject.t,
  actions: playerActions,
  hitActivated: bool,
  hitPower: float,
};

let create = (~id, ~position) => {
  {
    id,
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
    hitActivated: false,
    hitPower: 0.0,
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