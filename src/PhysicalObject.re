type t = {
  id: string,
  circle: Circle.t,
  mass: float,
  force: Vector.t,
  velocity: Vector.t,
  frictionCoefficient: float,
};

let friction = physicalObject => {
  physicalObject.velocity
  ->Vector.multiplyByScalar((-1.0) *. physicalObject.mass *. physicalObject.frictionCoefficient);
};

let acceleration = physicalObject => {
  let acceleration = Vector.divideByScalar(physicalObject.force, physicalObject.mass);
  let friction = friction(physicalObject);
  let effectiveAcceleration = Vector.add(acceleration, friction);
  effectiveAcceleration;
};

let updateVelocity = (physicalObject, time) => {
  let acceleration = acceleration(physicalObject);
  let velocity = Vector.add(physicalObject.velocity, acceleration->Vector.multiplyByScalar(time));
  {...physicalObject, velocity};
};

let updatePosition = (physicalObject, time) => {
  let position = Vector.add(physicalObject.circle.position, physicalObject.velocity->Vector.multiplyByScalar(time));
  {
    ...physicalObject,
    circle: {
      ...physicalObject.circle,
      position,
    },
  };
};

let updatePositionAndVelocity = (physicalObject, time) => {
  physicalObject->updatePosition(time)->updateVelocity(time);
};

let haveSameId = ({id: firstId}, {id: secondId}) => firstId == secondId;