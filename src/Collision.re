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

let velocitiesAfterElasticCollision =
    (objectA: PhysicalObject.t, objectB: PhysicalObject.t) => {
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
        *. Vector.dotProduct(
             Vector.subtract(v1, v2),
             Vector.subtract(c1, c2),
           )
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
        *. Vector.dotProduct(
             Vector.subtract(v2, v1),
             Vector.subtract(c2, c1),
           )
        /. Math.quad(Vector.length(Vector.subtract(c2, c1))),
      ),
    ),
  );
};

let findCollisions = (objects: Belt.List.t(PhysicalObject.t)) => {
  objects
  ->makePairs
  ->Belt.List.keep(((a, b)) => Circle.collide(a.circle, b.circle));
};

let toObjectsWithVelocitiesAfterCollision = collisions =>
  collisions
  ->Belt.List.map(((objectA, objectB)) => {
      let (velocityA, velocityB) =
        velocitiesAfterElasticCollision(objectA, objectB);
      [(objectA, velocityA), (objectB, velocityB)];
    })
  ->Belt.List.flatten;

let toObjectsWithResultantVelocities =
    (objectsWithVelocities: Belt.List.t((PhysicalObject.t, Vector.t))) =>
  objectsWithVelocities
  ->Belt.List.reduce(
      Belt.Map.String.empty, (acc, (previuosPhysicalObject, velocity)) =>
      acc->Belt.Map.String.update(
        previuosPhysicalObject.id,
        fun
        | Some(currentPhysicalObject) =>
          Some(
            currentPhysicalObject->PhysicalObject.setVelocity(
              Vector.add(currentPhysicalObject.velocity, velocity),
            ),
          )
        | None =>
          Some(previuosPhysicalObject->PhysicalObject.setVelocity(velocity)),
      )
    )
  ->Belt.Map.String.valuesToArray
  ->Belt.List.fromArray;

let simulateElasticCollisions = objects => {
  findCollisions(objects)
  ->toObjectsWithVelocitiesAfterCollision
  ->toObjectsWithResultantVelocities;
};