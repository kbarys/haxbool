let runInContext = (context, statements) =>
  Belt.List.forEach(statements, statement => statement(context));

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

let renderCircle =
    (
      canvasElement,
      circle: Circle.t,
      ~fillColor="#fff",
      ~strokeColor="#000",
      ~strokeWidth=4.0,
      (),
    ) => {
  Webapi.Canvas.Canvas2d.(
    runInContext(
      Webapi.Canvas.CanvasElement.getContext2d(canvasElement),
      [
        beginPath,
        arc(
          ~x=circle.position->fst /. Options.virtualWidth *. Options.width,
          ~y=circle.position->snd /. Options.virtualHeight *. Options.height,
          ~r=circle.radius /. Options.virtualWidth *. Options.width, // TODO: it should rendered as a elipsis
          ~startAngle=0.0,
          ~endAngle=Js.Math._PI *. 2.0,
          ~anticw=false,
        ),
        setFillStyle(_, String, fillColor),
        setStrokeStyle(_, String, strokeColor),
        lineWidth(_, strokeWidth),
        stroke,
        fill,
      ],
    )
  );
};

let clearCircle = (canvasElement, circle) => {
  renderCircle(
    canvasElement,
    circle,
    ~fillColor=Options.sceneColor,
    ~strokeColor=Options.sceneColor,
    ~strokeWidth=6.0,
    (),
  );
};

let clearScene = (canvasElement, circles) =>
  circles->Belt.List.forEach(clearCircle(canvasElement));

let renderPlayer = (canvasElement, player: Game.player) => {
  renderCircle(
    canvasElement,
    player.physicalObject.circle,
    ~strokeColor=player.actions.hit ? "#fff" : "#000",
    ~fillColor="#E56E56",
    (),
  );
};

let renderBall = (canvasElement, ball: Game.ball) =>
  renderCircle(canvasElement, ball.physicalObject.circle, ());

let render = (canvasElement, previousState: Game.state, state: Game.state) => {
  [
    previousState.ball.physicalObject.circle,
    ...previousState.players
       ->Belt.Map.String.valuesToArray
       ->Belt.List.fromArray
       ->Belt.List.map(player => player.physicalObject.circle),
  ]
  |> clearScene(canvasElement);
  state.players
  ->Belt.Map.String.valuesToArray
  ->Belt.List.fromArray
  ->Belt.List.forEach(renderPlayer(canvasElement));
  state.ball |> renderBall(canvasElement);
  state.collisionPoint
  ->Belt.Option.forEach(collisionPoint => {
      renderCircle(
        canvasElement,
        {position: collisionPoint, radius: 0.005},
        ~fillColor="red",
        (),
      )
    });
};