let update = () => {
  let (state, _) = MutableStore.use();
  state^.scene
  ->Belt.Option.flatMap(_, Webapi.Dom.Element.querySelector(".player"))
  ->Belt.Option.forEach(
      _,
      player => {
        Webapi.Dom.Element.setAttribute("cx", Js.Float.toString(state^.player.position->fst), player);
        Webapi.Dom.Element.setAttribute("cy", Js.Float.toString(state^.player.position->snd), player);
      },
    );
};

[@react.component]
let make = () => {
  let (state, dispatch) = MutableStore.use();
  React.useEffect(() => {
    let intervalId =
      Js.Global.setInterval(
        () => {
          let state = state^;
          let (horizontalSpeed, verticalSpeed) = state.player.speed;
          let options = state.options;
          let newHorizontalSpeed =
            switch (
              Belt.Map.getWithDefault(state.keys, Key.ArrowLeft, Key.Released),
              Belt.Map.getWithDefault(state.keys, Key.ArrowRight, Key.Released),
            ) {
            | (Key.Pressed, Key.Released) =>
              Js.Math.max_float(-. options.maxPlayerSpeed, horizontalSpeed -. options.playerAcceleration)
            | (Key.Released, Key.Pressed) =>
              Js.Math.min_float(horizontalSpeed +. options.playerAcceleration, options.maxPlayerSpeed)
            | (Key.Pressed, Key.Pressed)
            | (Key.Released, Key.Released) =>
              if (horizontalSpeed > 0.0) {
                Js.Math.max_float(horizontalSpeed -. options.playerBreak, 0.0);
              } else if (horizontalSpeed < 0.0) {
                Js.Math.min_float(horizontalSpeed +. options.playerBreak, 0.0);
              } else {
                0.0;
              }
            };
          let newVerticalSpeed =
            switch (
              Belt.Map.getWithDefault(state.keys, Key.ArrowUp, Key.Released),
              Belt.Map.getWithDefault(state.keys, Key.ArrowDown, Key.Released),
            ) {
            | (Key.Pressed, Key.Released) =>
              Js.Math.max_float(-. options.maxPlayerSpeed, verticalSpeed -. options.playerAcceleration)
            | (Key.Released, Key.Pressed) =>
              Js.Math.min_float(verticalSpeed +. options.playerAcceleration, options.maxPlayerSpeed)
            | (Key.Pressed, Key.Pressed)
            | (Key.Released, Key.Released) =>
              if (verticalSpeed > 0.0) {
                Js.Math.max_float(verticalSpeed -. options.playerBreak, 0.0);
              } else if (verticalSpeed < 0.0) {
                Js.Math.min_float(verticalSpeed +. options.playerBreak, 0.0);
              } else {
                0.0;
              }
            };
          if (horizontalSpeed != newHorizontalSpeed || verticalSpeed != newVerticalSpeed) {
            dispatch(MutableStore.PlayerSpeedChanged((newHorizontalSpeed, newVerticalSpeed)));
          };
          if (!Vector.equal(state.player.speed, (0.0, 0.0))) {
            dispatch(
              MutableStore.PlayerPositionChanged(
                Vector.add(state.player.position, (horizontalSpeed, verticalSpeed)),
              ),
            );
          };
          update();
        },
        8,
      );
    Some(() => Js.Global.clearInterval(intervalId));
  });
  React.useLayoutEffect(() => {
    let _ =
      Webapi.Dom.Document.querySelector(".scene", Webapi.Dom.document)
      ->Belt.Option.map(scene => dispatch(MutableStore.SceneLoaded(scene)));
    None;
  });
  let onBlur = _ => dispatch(MutableStore.FocusLost);
  <svg
    className="scene"
    width={Js.Float.toString(state^.options.width)}
    height={Js.Float.toString(state^.options.height)}
    tabIndex=0
    onBlur>
    <rect width="100%" height="100%" fill="#6F965E" />
    <circle
      className="player"
      cx={Js.Float.toString(state^.player.position->fst)}
      cy={Js.Float.toString(state^.player.position->snd)}
      r="2%"
      strokeWidth="0.25%"
      stroke="black"
      fill="#E56E56"
    />
  </svg>;
};