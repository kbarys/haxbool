let runInContext = (context, statements) => Belt.List.forEach(statements, statement => statement(context));

let init = canvasElement => {
  Webapi.Canvas.Canvas2d.(
    runInContext(
      Webapi.Canvas.CanvasElement.getContext2d(canvasElement),
      [
        setFillStyle(_, Webapi.Canvas.Canvas2d.String, Options.sceneColor),
        fillRect(~x=0.0, ~y=0.0, ~w=Options.width, ~h=Options.height),
      ],
    )
  );
};

let renderCircle = (canvasElement, circle: Circle.t, ~fillColor="#fff", ~strokeColor="#000", ~strokeWidth=0.01, ()) => {
  Webapi.Canvas.Canvas2d.(
    runInContext(
      Webapi.Canvas.CanvasElement.getContext2d(canvasElement),
      [
        beginPath,
        arc(
          ~x=circle.position->fst /. Options.virtualWidth *. Options.width,
          ~y=circle.position->snd /. Options.virtualHeight *. Options.height,
          ~r=(circle.radius -. strokeWidth /. 2.0) /. Options.virtualWidth *. Options.width, // TODO: it should be rendered as an elipsis
          ~startAngle=0.0,
          ~endAngle=Js.Math._PI *. 2.0,
          ~anticw=false,
        ),
        setFillStyle(_, String, fillColor),
        setStrokeStyle(_, String, strokeColor),
        lineWidth(_, strokeWidth /. Options.virtualWidth *. Options.width),
        stroke,
        fill,
      ],
    )
  );
};

let clearCircle = (circle: Circle.t, canvasElement) => {
  renderCircle(
    canvasElement,
    {...circle, radius: circle.radius *. 1.5},
    ~fillColor=Options.sceneColor,
    ~strokeColor=Options.sceneColor,
    ~strokeWidth=0.0,
    (),
  );
};

let clearScene = (state: Game.state, canvasElement) => {
  let circles = [state.ball.circle, ...state.players->Belt.List.map(player => player.physicalObject.circle)];
  circles->Belt.List.forEach(__x => clearCircle(__x, canvasElement));
};

let renderPlayer = (player: Player.t, canvasElement) => {
  renderCircle(
    canvasElement,
    player.physicalObject.circle,
    ~strokeColor=player.actions.hit ? "#fff" : "#000",
    ~fillColor="#E56E56",
    ~strokeWidth=Options.playerStrokeSize,
    (),
  );
};

let renderBall = (ball: PhysicalObject.t, canvasElement) =>
  renderCircle(canvasElement, ball.circle, ~strokeWidth=Options.ballStrokeSize, ());

let render = (canvasElement, previousState: Game.state, state: Game.state) => {
  runInContext(
    canvasElement,
    [clearScene(previousState), renderBall(state.ball), ...state.players->Belt.List.map(renderPlayer)],
  );
};