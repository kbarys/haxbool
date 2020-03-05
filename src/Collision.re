open Belt;

type t = (PhysicalObject.t, PhysicalObject.t);

let makePairs = objects => {
  let rec makePairs_ = (pairs, remaining) => {
    switch (remaining) {
    | []
    | [_] => pairs
    | [head, ...rest] => makePairs_(List.concat(pairs, List.map(rest, one => (head, one))), rest)
    };
  };
  makePairs_([], objects);
};

let findCollisions =
    (objects: Map.String.t(PhysicalObject.t), ~collisionDetector: (Circle.t, Circle.t, ~precission: float) => bool) => {
  objects
  ->Map.String.valuesToArray
  ->List.fromArray
  ->makePairs
  ->List.keep(((a, b)) => collisionDetector(a.circle, b.circle, ~precission=0.000000001));
};

let velocitiesAfterElasticCollision = ((objectA, objectB)) => {
  open PhysicalObject;
  let {velocity: v1, mass: m1, circle: {position: c1}} = objectA;
  let {velocity: v2, mass: m2, circle: {position: c2}} = objectB;
  let u1 =
    Vector.(
      subtract(
        v1,
        multiplyByScalar(
          subtract(c1, c2),
          2.0
          *. m2
          /. (m1 +. m2)
          *. dotProduct(subtract(v1, v2), subtract(c1, c2))
          /. Math.quad(length(subtract(c1, c2))),
        ),
      )
    );
  let u2 =
    Vector.(
      subtract(
        v2,
        multiplyByScalar(
          subtract(c2, c1),
          2.0
          *. m1
          /. (m1 +. m2)
          *. dotProduct(subtract(v2, v1), subtract(c2, c1))
          /. Math.quad(length(subtract(c2, c1))),
        ),
      )
    );
  if (Vector.(Js.Math.abs_float(length(v1) -. length(v2)) < 0.1) && false) {
    let averageLength = Vector.((length(v1) +. length(v2)) /. 2.0);
    Vector.(adjustLength(u1, ~length=averageLength *. 0.5), adjustLength(u2, ~length=averageLength));
  } else {
    (u1, u2);
  };
};

let zeroVelocityOfCollidingObjects = (objectById, collisions) => {
  objectById->Map.String.map(object_ =>
    List.some(collisions, ((objectA, objectB)) =>
      PhysicalObject.haveSameId(object_, objectA) || PhysicalObject.haveSameId(object_, objectB)
    )
      ? {...object_, velocity: Vector.zero} : object_
  );
};

let withVelocitiesAfterCollisions = (initialObjectById: Map.String.t(PhysicalObject.t)) => {
  let collisions = findCollisions(initialObjectById, ~collisionDetector=Circle.contact);
  List.reduce(
    collisions,
    zeroVelocityOfCollidingObjects(initialObjectById, collisions),
    (objectById, collision) => {
      let updateObjectVelocity = (objects, {id}: PhysicalObject.t, velocity) =>
        Map.String.update(
          objects,
          id,
          Option.map(_, object_ =>
            {...object_, PhysicalObject.velocity: Vector.add(velocity, object_.PhysicalObject.velocity)}
          ),
        );
      let (velocityAfterCollisionA, velocityAfterCollisionB) = velocitiesAfterElasticCollision(collision);
      let (objectA, objectB) = collision;
      objectById
      ->updateObjectVelocity(objectA, velocityAfterCollisionA)
      ->updateObjectVelocity(objectB, velocityAfterCollisionB);
    },
  );
};