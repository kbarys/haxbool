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

let multiply = ((x, y), ~by) => (x *. by, y *. by);

let min = ((a, b), (c, d)) => (
  Js.Math.min_float(a, c),
  Js.Math.min_float(b, d),
);
let max = ((a, b), (c, d)) => (
  Js.Math.max_float(a, c),
  Js.Math.max_float(b, d),
);
