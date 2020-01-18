type keyType =
  | ArrowUp
  | ArrowDown
  | ArrowLeft
  | ArrowRight;
type t = keyType;
type state =
  | Pressed
  | Released;
let toString =
  fun
  | ArrowUp => "ArrowUp"
  | ArrowDown => "ArrowDown"
  | ArrowLeft => "ArrowLeft"
  | ArrowRight => "ArrowRight";
let fromString =
  fun
  | "ArrowUp" => Some(ArrowUp)
  | "ArrowDown" => Some(ArrowDown)
  | "ArrowLeft" => Some(ArrowLeft)
  | "ArrowRight" => Some(ArrowRight)
  | _ => None;
module Comparator =
  Belt.Id.MakeComparable({
    type t = keyType;
    let cmp = (first, second) => compare(toString(first), toString(second));
  });