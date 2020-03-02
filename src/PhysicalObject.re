type t = {
  id: string,
  circle: Circle.t,
  mass: float,
  force: Vector.t,
  velocity: Vector.t,
  acceleration: Vector.t,
  frictionCoefficient: float,
};

let friction = physicalObject => {
  physicalObject.velocity
  ->Vector.multiplyByScalar((-1.0) *. physicalObject.mass *. physicalObject.frictionCoefficient);
};

let keepPositionInBounds = position => {
  position
  ->Vector.max((Options.playerRadius, Options.playerRadius))
  ->Vector.min((Options.virtualWidth -. Options.playerRadius, Options.virtualHeight -. Options.playerRadius));
};

let updateVelocity = (physicalObject, time) => {
  let acceleration = Vector.divideByScalar(physicalObject.force, physicalObject.mass);
  let friction = friction(physicalObject);
  let effectiveAcceleration = Vector.add(acceleration, friction);
  let velocity = Vector.add(physicalObject.velocity, effectiveAcceleration->Vector.multiplyByScalar(time));
  {...physicalObject, velocity};
};

let updatePosition = (physicalObject, time) => {
  let position = Vector.add(physicalObject.circle.position, physicalObject.velocity->Vector.multiplyByScalar(time));
  {...physicalObject, circle: physicalObject.circle->Circle.setPosition(position)};
};

let update = (physicalObject, time) => {
  physicalObject->updatePosition(time)->updateVelocity(time);
};

let setForce = (physicalObject, force) => {...physicalObject, force};

let setVelocity = (physicalObject, velocity) => {...physicalObject, velocity};

let haveSameId = ({id: firstId}, {id: secondId}) => firstId == secondId;