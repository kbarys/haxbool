type t = (float, float);

let add = (a, b) => {
  let (ax, ay) = a;
  let (bx, by) = b;
  (ax +. bx, ay +. by);
};

let equal = (a, b) => {
  let (ax, ay) = a;
  let (bx, by) = b;
  ax == bx && ay == by;
};