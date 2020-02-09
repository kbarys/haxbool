type t = {
  ids: (string, string),
  point: Vector.t,
};

let makePairs = objects => {
  let rec makePairs_ = (pairs, remaining) => {
    switch (remaining) {
    | []
    | [_] => pairs
    | [head, ...rest] =>
      makePairs_(
        Belt.List.concat(pairs, Belt.List.map(rest, one => (head, one))),
        rest,
      )
    };
  };
  makePairs_([], objects);
};

let findCollisions = (objects: Belt.List.t(PhysicalObject.t)) => {
  objects
  ->makePairs
  ->Belt.List.map(((objectA, objectB)) =>
      Circle.contactPoint(objectA.circle, objectB.circle)
      ->Belt.Option.map(contactPoint =>
          {ids: (objectA.id, objectB.id), point: contactPoint}
        )
    )
  ->Belt.List.keep(Belt.Option.isSome)
  ->Belt.List.map(Belt.Option.getExn);
};