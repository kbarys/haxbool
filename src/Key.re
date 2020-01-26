type keyType =
  | ArrowUp
  | ArrowDown
  | ArrowLeft
  | ArrowRight
  | Space;
type t = keyType;
type state =
  | Pressed
  | Released;
let toString =
  fun
  | ArrowUp => "ArrowUp"
  | ArrowDown => "ArrowDown"
  | ArrowLeft => "ArrowLeft"
  | ArrowRight => "ArrowRight"
  | Space => " ";
let fromString =
  fun
  | "ArrowUp" => Some(ArrowUp)
  | "ArrowDown" => Some(ArrowDown)
  | "ArrowLeft" => Some(ArrowLeft)
  | "ArrowRight" => Some(ArrowRight)
  | " " => Some(Space)
  | _ => None;
module Comparator =
  Belt.Id.MakeComparable({
    type t = keyType;
    let cmp = (first, second) => compare(toString(first), toString(second));
  });