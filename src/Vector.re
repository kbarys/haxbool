type t = (float, float);

let equals = ((a, b), (c, d)) => a == b && c == d;

let add = ((a, b), (c, d)) => (a +. c, b +. d);

let subtract = ((a, b), (c, d)) => (a -. c, b -. d);

let multiplyByScalar = ((x, y), by) => (x *. by, y *. by);

let divideByScalar = ((x, y), by) => (x /. by, y /. by);

let multiply = ((a, b), (c, d)) => (a *. c, b *. d);

let divide = ((a, b), (c, d)) => (a /. c, b /. d);

let min = ((a, b), (c, d)) => (Js.Math.min_float(a, c), Js.Math.min_float(b, d));

let max = ((a, b), (c, d)) => (Js.Math.max_float(a, c), Js.Math.max_float(b, d));

let zero = (0.0, 0.0);

let toString = ((x, y)) => {j|($x, $y)|j};
let toStringFixed = ((x, y), ~digits) => {
  let x = Js.Float.toFixedWithPrecision(x, ~digits);
  let y = Js.Float.toFixedWithPrecision(y, ~digits);
  {j|($x, $y)|j};
};

let lift = (x: float) => (x, x);

let distance = ((x1, y1), (x2, y2)) => Js.Math.sqrt(Math.quad(x1 -. x2) +. Math.quad(y1 -. y2));

let straightEquationCoefficients = ((x1, y1), (x2, y2)) =>
  if (x1 == x2) {
    (1.0, 0.0, -. x1);
  } else if (y1 == y2) {
    (0.0, 1.0, -. y1);
  } else {
    let a = (y1 -. y2) /. (x1 -. x2);
    let c = y1 -. a *. x1;
    (-. a, 1.0, -. c);
  };

let length = ((x, y)) => Js.Math.sqrt(Math.quad(x) +. Math.quad(y));

let dotProduct = ((x1, y1), (x2, y2)) => x1 *. x2 +. y1 *. y2;

let adjustLength = (v, ~length as l) => multiplyByScalar(v, length(v) /. l);