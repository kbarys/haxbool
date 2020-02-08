type t = {
  position: Vector.t,
  radius: float,
};

let setPosition = (circle, newPosition) => {
  ...circle,
  position: newPosition,
};
