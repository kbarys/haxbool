let approxEqual = (a, b, ~precission) =>
  Js.Math.abs_float(a -. b) < precission;

let decrease = (speed, ~by) =>
  if (speed == 0.0) {
    0.0;
  } else if (speed < 0.0) {
    Js.Math.min_float(speed +. by, 0.0);
  } else {
    Js.Math.max_float(speed -. by, 0.0);
  };

let increase = (speed, ~by) =>
  (speed +. by)
  ->Js.Math.min_float(Options.maxPlayerSpeed)
  ->Js.Math.max_float(-. Options.maxPlayerSpeed);
