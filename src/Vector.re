type t = (float, float);

let add = ((a, b), (c, d)) => (a +. c, b +. d);

let subtract = ((a, b), (c, d)) => (a -. c, b -. d);

let multiplyByScalar = ((x, y), by) => (x *. by, y *. by);

let divideByScalar = ((x, y), by) => (x /. by, y /. by);

let zero = (0.0, 0.0);

let toString = ((x, y)) => {j|($x, $y)|j};

let toStringFixed = ((x, y), ~digits) => {
  let x = Js.Float.toFixedWithPrecision(x, ~digits);
  let y = Js.Float.toFixedWithPrecision(y, ~digits);
  {j|($x, $y)|j};
};

let distance = ((x1, y1), (x2, y2)) => Js.Math.sqrt(Math.quad(x1 -. x2) +. Math.quad(y1 -. y2));

let length = ((x, y)) => Js.Math.sqrt(Math.quad(x) +. Math.quad(y));

let dotProduct = ((x1, y1), (x2, y2)) => x1 *. x2 +. y1 *. y2;

let adjustLength = (v, ~length as l) => multiplyByScalar(v, l /. length(v));

let normalize = v => divideByScalar(v, length(v));