type t = {
  position: Vector.t,
  radius: float,
};

let updatePosition = (circle, newPosition) => {
  ...circle,
  position: newPosition,
};
