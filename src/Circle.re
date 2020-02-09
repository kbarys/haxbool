type t = {
  position: Vector.t,
  radius: float,
};

let setPosition = (circle, newPosition) => {
  ...circle,
  position: newPosition,
};

let collide = (a, b) => {
  let distanceBetweenCenters = Vector.distance(a.position, b.position);
  distanceBetweenCenters <= a.radius +. b.radius;
};