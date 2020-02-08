type t = (float, float);

let equals = ((a, b), (c, d)) => a == b && c == d;

let add = ((a, b), (c, d)) => (a +. c, b +. d);

let subtract = ((a, b), (c, d)) => (a -. c, b -. d);

let multiplyByScalar = ((x, y), by) => (x *. by, y *. by);

let divideByScalar = ((x, y), by) => (x /. by, y /. by);

let multiply = ((a, b), (c, d)) => (a *. c, b *. d);

let divide = ((a, b), (c, d)) => (a /. c, b /. d);

let min = ((a, b), (c, d)) => (
  Js.Math.min_float(a, c),
  Js.Math.min_float(b, d),
);

let max = ((a, b), (c, d)) => (
  Js.Math.max_float(a, c),
  Js.Math.max_float(b, d),
);

let zero = (0.0, 0.0);

let toString = ((x, y)) => {j|($x, $y)|j};
let toStringFixed = ((x, y), ~digits) => {
  let x = Js.Float.toFixedWithPrecision(x, ~digits);
  let y = Js.Float.toFixedWithPrecision(y, ~digits);
  {j|($x, $y)|j};
};

let lift = (x: float) => (x, x);