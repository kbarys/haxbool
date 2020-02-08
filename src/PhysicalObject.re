type t = {
  circle: Circle.t,
  mass: float,
  force: Vector.t,
  velocity: Vector.t,
  acceleration: Vector.t,
};

let friction = physicalObject => {
  let frictionCoefficient = 0.05;
  physicalObject.velocity
  ->Vector.multiplyByScalar(
      (-1.0) *. physicalObject.mass *. frictionCoefficient,
    );
};

let keepPositionInBounds = position => {
  Js.log(position->Vector.toString);
  position
  ->Vector.max((Options.playerRadius, Options.playerRadius))
  ->Vector.min((
      Options.virtualWidth -. Options.playerRadius,
      Options.virtualHeight -. Options.playerRadius,
    ));
};

let updateAcceleration = physicalObject => {
  let acceleration =
    Vector.divideByScalar(physicalObject.force, physicalObject.mass);
  let friction = friction(physicalObject);
  let effectiveAcceleration = Vector.add(acceleration, friction);
  {...physicalObject, acceleration: effectiveAcceleration};
};

let updateVelocity = (physicalObject, time) => {
  let velocity =
    Vector.add(
      physicalObject.velocity,
      physicalObject.acceleration->Vector.multiplyByScalar(time),
    );
  {...physicalObject, velocity};
};

let updatePosition = (physicalObject, time) => {
  let position =
    Vector.add(
      physicalObject.circle.position,
      physicalObject.velocity
      ->Vector.multiplyByScalar(time)
      ->Vector.multiply((1.0, Options.virtualWidth /. Options.virtualHeight)),
    );
  {
    ...physicalObject,
    circle: physicalObject.circle->Circle.setPosition(position),
  };
};

let setForce = (physicalObject, force) => {...physicalObject, force};