type t = {
  position: Vector.t,
  radius: float,
};

let contact = (a, b, ~precission) => {
  let distanceBetweenCenters = Vector.distance(a.position, b.position);
  Math.approxEqual(distanceBetweenCenters, a.radius +. b.radius, ~precission);
};

let collide = (a, b, ~precission) => {
  let distanceBetweenCenters = Vector.distance(a.position, b.position);
  let sumOfRadiuses = a.radius +. b.radius;
  sumOfRadiuses -. distanceBetweenCenters > precission;
};
