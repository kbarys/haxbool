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

let renderArc =
    (
      canvasElement,
      ~center: Vector.t,
      ~radius,
      ~startAngle=0.0,
      ~endAngle=Js.Math._PI *. 2.0,
      ~fillColor="none",
      ~strokeColor="#000",
      ~strokeWidth=0.01,
      (),
    ) => {
  Webapi.Canvas.Canvas2d.(
    runInContext(
      Webapi.Canvas.CanvasElement.getContext2d(canvasElement),
      [
        beginPath,
        arc(
          ~x=center->fst /. Options.virtualWidth *. Options.width,
          ~y=center->snd /. Options.virtualHeight *. Options.height,
          ~r=(radius -. strokeWidth /. 2.0) /. Options.virtualWidth *. Options.width, // TODO: it should be rendered as an elipsis
          ~startAngle,
          ~endAngle,
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

let renderCircle = (canvasElement, circle: Circle.t, ~fillColor="#fff", ~strokeColor="#000", ~strokeWidth=0.01, ()) => {
  renderArc(
    canvasElement,
    ~center=circle.position,
    ~radius=circle.radius,
    ~fillColor,
    ~strokeColor,
    ~strokeWidth,
    (),
  );
};

let clearCircle = (circle: Circle.t, canvasElement) => {
  renderCircle(
    canvasElement,
    {...circle, radius: circle.radius *. 3.0},
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
    ~strokeColor="#000",
    ~fillColor="#E56E56",
    ~strokeWidth=Options.playerStrokeSize,
    (),
  );
  if (player.hitPower > 0.0) {
    renderArc(
      canvasElement,
      ~center=player.physicalObject.circle.position,
      ~radius=player.physicalObject.circle.radius -. 0.003,
      ~startAngle=(-0.5) *. Js.Math._PI,
      ~endAngle=2.0 *. Js.Math._PI *. player.hitPower -. 0.5 *. Js.Math._PI,
      ~strokeColor="#fff",
      ~fillColor="transparent",
      ~strokeWidth=0.005,
      (),
    );
  };
};

let renderBall = (ball: PhysicalObject.t, canvasElement) =>
  renderCircle(canvasElement, ball.circle, ~strokeWidth=Options.ballStrokeSize, ());

let render = (canvasElement, previousState: Game.state, state: Game.state) => {
  runInContext(
    canvasElement,
    [clearScene(previousState), renderBall(state.ball), ...state.players->Belt.List.map(renderPlayer)],
  );
};