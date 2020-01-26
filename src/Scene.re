[@react.component]
let make = () => {
  let size = 1000.0;
  let width = size;
  let height = size /. 2.0;
  let maxSpeed = 0.001 *. size;
  let acceleration = 0.00001 *. size;
  let break = 0.000001 *. size;
  let (state, dispatch) = Store.use();
  let ((horizontalSpeed, verticalSpeed) as speed, setSpeed) = React.useState(() => (0.0, 0.0));
  let ((x, y), setPosition) = React.useState(() => (width *. 0.5, height *. 0.5));
  React.useEffect2(
    () => {
      let intervalId =
        Js.Global.setInterval(
          () => {
            let newHorizontalSpeed =
              switch (
                Belt.Map.getWithDefault(state.keys, Key.ArrowLeft, Key.Released),
                Belt.Map.getWithDefault(state.keys, Key.ArrowRight, Key.Released),
              ) {
              | (Key.Pressed, Key.Released) => Js.Math.max_float(-. maxSpeed, horizontalSpeed -. acceleration)
              | (Key.Released, Key.Pressed) => Js.Math.min_float(horizontalSpeed +. acceleration, maxSpeed)
              | (Key.Pressed, Key.Pressed)
              | (Key.Released, Key.Released) =>
                if (horizontalSpeed > 0.0) {
                  Js.Math.max_float(horizontalSpeed -. break, 0.0);
                } else if (horizontalSpeed < 0.0) {
                  Js.Math.min_float(horizontalSpeed +. break, 0.0);
                } else {
                  0.0;
                }
              };
            let newVerticalSpeed =
              switch (
                Belt.Map.getWithDefault(state.keys, Key.ArrowUp, Key.Released),
                Belt.Map.getWithDefault(state.keys, Key.ArrowDown, Key.Released),
              ) {
              | (Key.Pressed, Key.Released) => Js.Math.max_float(-. maxSpeed, verticalSpeed -. acceleration)
              | (Key.Released, Key.Pressed) => Js.Math.min_float(verticalSpeed +. acceleration, maxSpeed)
              | (Key.Pressed, Key.Pressed)
              | (Key.Released, Key.Released) =>
                if (verticalSpeed > 0.0) {
                  Js.Math.max_float(verticalSpeed -. break, 0.0);
                } else if (verticalSpeed < 0.0) {
                  Js.Math.min_float(verticalSpeed +. break, 0.0);
                } else {
                  0.0;
                }
              };
            if (horizontalSpeed != newHorizontalSpeed || verticalSpeed != newVerticalSpeed) {
              setSpeed(_ => (newHorizontalSpeed, newVerticalSpeed));
            };
            setPosition(((x, y)) => (x +. horizontalSpeed, y +. verticalSpeed));
          },
          1,
        );
      Some(() => Js.Global.clearInterval(intervalId));
    },
    (state.keys, speed),
  );
  let onBlur = _ => dispatch(Store.FocusLost);
  <svg className="scene" width={Js.Float.toString(width)} height={Js.Float.toString(height)} tabIndex=0 onBlur>
    <rect width="100%" height="100%" fill="#6F965E" />
    <circle cx={Js.Float.toString(x)} cy={Js.Float.toString(y)} r="2%" strokeWidth="0.25%" stroke="black" fill="#E56E56" />
  </svg>;
};