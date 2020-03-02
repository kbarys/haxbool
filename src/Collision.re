open Belt;

type t = (PhysicalObject.t, PhysicalObject.t);

let makePairs = objects => {
  let rec makePairs_ = (pairs, remaining) => {
    switch (remaining) {
    | []
    | [_] => pairs
    | [head, ...rest] => makePairs_(Belt.List.concat(pairs, Belt.List.map(rest, one => (head, one))), rest)
    };
  };
  makePairs_([], objects);
};

let findCollisions = (objects: Map.String.t(PhysicalObject.t)) => {
  objects
  ->Map.String.valuesToArray
  ->List.fromArray
  ->makePairs
  ->Belt.List.keep(((a, b)) => Circle.contact(a.circle, b.circle, ~precission=0.00000000001));
};

let velocitiesAfterElasticCollision = ((objectA, objectB)) => {
  open PhysicalObject;
  let {velocity: v1, mass: m1, circle: {position: c1}} = objectA;
  let {velocity: v2, mass: m2, circle: {position: c2}} = objectB;
  (
    Vector.subtract(
      v1,
      Vector.multiplyByScalar(
        Vector.subtract(c1, c2),
        2.0
        *. m2
        /. (m1 +. m2)
        *. Vector.dotProduct(Vector.subtract(v1, v2), Vector.subtract(c1, c2))
        /. Math.quad(Vector.length(Vector.subtract(c1, c2))),
      ),
    ),
    Vector.subtract(
      v2,
      Vector.multiplyByScalar(
        Vector.subtract(c2, c1),
        2.0
        *. m1
        /. (m1 +. m2)
        *. Vector.dotProduct(Vector.subtract(v2, v1), Vector.subtract(c2, c1))
        /. Math.quad(Vector.length(Vector.subtract(c2, c1))),
      ),
    ),
  );
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
  let collisions = findCollisions(initialObjectById);
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