[@react.component]
let make = () => {
  React.useLayoutEffect(() => {
    Input.track();
    None;
  });
  React.useLayoutEffect(() => {
    switch (Webapi.Dom.Document.querySelector(".scene", Webapi.Dom.document)) {
    | Some(scene) =>
      Scene.init(scene);
      Loop.loop(0.0, ~previousTimestamp=0.0, scene, Game.initState);
    | None => Js.log("Canvas element (.scene) was not found")
    };
    None;
  });
  <canvas
    className="scene"
    width={Js.Float.toString(Options.width)}
    height={Js.Float.toString(Options.height)}
    tabIndex=0
  />;
};
