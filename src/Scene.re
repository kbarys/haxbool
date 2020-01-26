let init = () => {
  module Canvas = Webapi.Canvas.Canvas2d;
  let (state, _) = MutableStore.use();
  let state = state^;
  state.scene
  ->Belt.Option.map(_, Webapi.Canvas.CanvasElement.getContext2d)
  ->Belt.Option.forEach(context => {
      Canvas.setFillStyle(context, Canvas.String, "#6F965E");
      Canvas.fillRect(~x=0.0, ~y=0.0, ~w=state.options.width, ~h=state.options.height, context);
    });
};
let runInContext = (context, statements) => Belt.List.forEach(statements, statement => statement(context));
let update = () => {
  let (state, _) = MutableStore.use();
  let {MutableStore.State.options, MutableStore.State.player, MutableStore.State.scene} = state^;
  scene
  ->Belt.Option.map(_, Webapi.Canvas.CanvasElement.getContext2d)
  ->Belt.Option.forEach(context => {
      Webapi.Canvas.Canvas2d.(
        runInContext(
          context,
          [
            setFillStyle(_, String, "#6F965E"),
            fillRect(
              ~x=player.position->fst -. options.playerRadius *. 1.5,
              ~y=player.position->snd -. options.playerRadius *. 1.5,
              ~w=options.playerRadius *. 3.0,
              ~h=options.playerRadius *. 3.0,
            ),
            beginPath,
            arc(
              ~x=player.position->fst,
              ~y=player.position->snd,
              ~r=options.playerRadius,
              ~startAngle=0.0,
              ~endAngle=Js.Math._PI *. 2.0,
              ~anticw=false,
            ),
            setFillStyle(_, String, "#E56E56"),
            setStrokeStyle(_, String, player.hit ? "#fff" : "#000"),
            lineWidth(_, 4.0),
            stroke,
            fill,
          ],
        )
      )
    });
};

[@react.component]
let make = () => {
  let (state, dispatch) = MutableStore.use();
  React.useEffect(() => {
    init();
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
          dispatch(
            MutableStore.PlayerHitChanged(
              state.keys->Belt.Map.getWithDefault(Key.Space, Key.Released) === Key.Pressed,
            ),
          );
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
  <canvas
    className="scene"
    width={Js.Float.toString(state^.options.width)}
    height={Js.Float.toString(state^.options.height)}
    tabIndex=0
    onBlur
  />;
};