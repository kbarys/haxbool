let quad = x => Js.Math.pow_float(~base=x, ~exp=2.0);

let approxEqual = (a, b, ~precission) => {
  a -. b <= precission;
};