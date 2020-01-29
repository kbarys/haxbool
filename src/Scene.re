let runInContext = (context, statements) =>
  Belt.List.forEach(statements, statement => statement(context));

let init = canvasElement => {
  Webapi.Canvas.Canvas2d.(
    runInContext(
      Webapi.Canvas.CanvasElement.getContext2d(canvasElement),
      [
        setFillStyle(_, Webapi.Canvas.Canvas2d.String, "#6F965E"),
        fillRect(~x=0.0, ~y=0.0, ~w=Options.width, ~h=Options.height),
      ],
    )
  );
};

let renderPlayer = (canvasElement, player: Game.player) => {
  Webapi.Canvas.Canvas2d.(
    runInContext(
      Webapi.Canvas.CanvasElement.getContext2d(canvasElement),
      [
        setFillStyle(_, String, "#6F965E"),
        fillRect(
          ~x=player.position->fst -. Options.playerRadius *. 1.5,
          ~y=player.position->snd -. Options.playerRadius *. 1.5,
          ~w=Options.playerRadius *. 3.0,
          ~h=Options.playerRadius *. 3.0,
        ),
        beginPath,
        arc(
          ~x=player.position->fst,
          ~y=player.position->snd,
          ~r=Options.playerRadius,
          ~startAngle=0.0,
          ~endAngle=Js.Math._PI *. 2.0,
          ~anticw=false,
        ),
        setFillStyle(_, String, "#E56E56"),
        setStrokeStyle(_, String, player.actions.hit ? "#fff" : "#000"),
        lineWidth(_, 4.0),
        stroke,
        fill,
      ],
    )
  );
};

let render = (canvasElement, state) => {
  let {Game.players} = state;
  players
  ->Belt.Map.String.valuesToArray
  ->Belt.List.fromArray
  ->Belt.List.forEach(renderPlayer(canvasElement));
};
