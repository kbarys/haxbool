[@react.component]
let make = () => {
  React.useLayoutEffect(() => {
    Input.track();
    None;
  });
  React.useLayoutEffect(() => {
    switch (Webapi.Dom.Document.querySelector(".scene", Webapi.Dom.document)) {
    | Some(scene) => Loop.start(scene)
    | None => Js.log("Canvas element (.scene) was not found")
    };
    None;
  });
  <>
    <canvas
      className="scene"
      width={Js.Float.toString(Options.width)}
      height={Js.Float.toString(Options.height)}
      tabIndex=0
    />
    <div className="debug">
      <div> {React.string("Force: ")} <span className="force-value" /> </div>
      <div> {React.string("Acceleration: ")} <span className="acceleration-value" /> </div>
      <div> {React.string("Friction: ")} <span className="friction-value" /> </div>
      <div> {React.string("Velocity: ")} <span className="velocity-value" /> </div>
      <div> {React.string("Hit: ")} <span className="hit" /> </div>
    </div>
  </>;
};