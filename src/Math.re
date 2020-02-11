let quad = x => Js.Math.pow_float(~base=x, ~exp=2.0);

let solveQuadraticEquation = ((a, b, c)) => {
  let delta = quad(b) -. 4.0 *. a *. c;
  if (delta < 0.0) {
    [];
  } else if (delta == 0.0) {
    [-. b /. (2.0 *. a)];
  } else {
    [Js.Math.sqrt(delta), -. Js.Math.sqrt(delta)]
    ->Belt.List.map(x => (x -. b) /. (2.0 *. a));
  };
};
