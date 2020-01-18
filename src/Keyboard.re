
[@react.component]
let make = (~children) => {
  <div className="keyboard" onKeyDown=Js.log tabIndex=0> children </div>;
};