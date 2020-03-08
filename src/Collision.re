open Belt;

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

let objectsCollide = (a: PhysicalObject.t, b: PhysicalObject.t) => {
  let distanceBetweenCenters = Vector.distance(a.circle.position, b.circle.position);
  let sumOfRadiuses = a.circle.radius +. b.circle.radius;
  let collisionTreshold = Js.Math.pow_float(~base=10.0, ~exp=-5.0);
  sumOfRadiuses -. distanceBetweenCenters > collisionTreshold;
};

let findCollisions = (objects: Map.String.t(PhysicalObject.t)) => {
  objects
  ->Map.String.valuesToArray
  ->List.fromArray
  ->makePairs
  ->List.keep(((a, b)) => objectsCollide(a, b))
  ->List.map(((a, b)) => (a.id, b.id));
};

let velocityAfterCollision = ((v1, m1, c1), (v2, m2, c2)) => {
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
};

let velocitiesAfterElasticCollision = ((objectA, objectB)) => {
  open PhysicalObject;
  let {velocity: v1, mass: m1, circle: {position: c1}} = objectA;
  let {velocity: v2, mass: m2, circle: {position: c2}} = objectB;
  let u1 = velocityAfterCollision((v1, m1, c1), (v2, m2, c2));
  let u2 = velocityAfterCollision((v2, m2, c2), (v1, m1, c1));
  (u1, u2);
};

let zeroVelocityOfCollidingObjects = (objectById, collisions) => {
  objectById->Map.String.map(object_ =>
    List.some(collisions, ((objectA, objectB)) =>
      PhysicalObject.haveSameId(object_, objectA) || PhysicalObject.haveSameId(object_, objectB)
    )
      ? {...object_, velocity: Vector.zero} : object_
  );
};

let objectsContact = ((a: PhysicalObject.t, b: PhysicalObject.t)) => {
  let contactPrecission = Js.Math.pow_float(~base=10.0, ~exp=-15.0);
  Math.approxEqual(
    a.circle.radius +. b.circle.radius,
    Vector.distance(a.circle.position, b.circle.position),
    ~precission=contactPrecission,
  );
};

let collidingObjectsFromCollision = (objectById, collision) => {
  let (idA, idB) = collision;
  (Map.String.getExn(objectById, idA), Map.String.getExn(objectById, idB));
};

let withVelocitiesAfterCollisions = (initialObjectById, hypotheticalCollisions) => {
  let collidingObjects =
    hypotheticalCollisions->List.map(collidingObjectsFromCollision(initialObjectById))->List.keep(objectsContact);
  List.reduce(
    collidingObjects,
    zeroVelocityOfCollidingObjects(initialObjectById, collidingObjects),
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