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

let zeroVelocityOfCollidingObjects = (objectById, collisions) => {
  objectById->Map.String.map(object_ =>
    List.some(collisions, ((objectA, objectB)) =>
      PhysicalObject.haveSameId(object_, objectA) || PhysicalObject.haveSameId(object_, objectB)
    )
      ? {...object_, velocity: Vector.zero} : object_
  );
};

let objectsContact = ((a: PhysicalObject.t, b: PhysicalObject.t)) => {
  let contactprecision = Js.Math.pow_float(~base=10.0, ~exp=-15.0);
  Math.approxEqual(
    a.circle.radius +. b.circle.radius,
    Vector.distance(a.circle.position, b.circle.position),
    ~precision=contactprecision,
  );
};

let collidingObjectsFromCollision = (objectById, collision) => {
  let (idA, idB) = collision;
  (Map.String.getExn(objectById, idA), Map.String.getExn(objectById, idB));
};

let velocitiesAfterElasticCollision = ((objectA, objectB)) => {
  open Vector;
  let {PhysicalObject.velocity: v1, mass: m1, circle: {position: c1}} = objectA;
  let {PhysicalObject.velocity: v2, mass: m2, circle: {position: c2}} = objectB;
  let normal = subtract(c1, c2);
  let normalUnit = normalize(normal);
  let tangentUnit = (-. normalUnit->snd, normalUnit->fst);
  let v1n = dotProduct(normalUnit, v1);
  let v1t = dotProduct(tangentUnit, v1);
  let v2n = dotProduct(normalUnit, v2);
  let v2t = dotProduct(tangentUnit, v2);
  let v1t' = v1t;
  let v1n' = (m1 *. v1n +. m2 *. v2n) /. (m1 +. m2);
  let v2t' = v2t;
  let v2n' = v1n';
  let v1' = add(multiplyByScalar(normalUnit, v1n'), multiplyByScalar(tangentUnit, v1t'));
  let v2' = add(multiplyByScalar(normalUnit, v2n'), multiplyByScalar(tangentUnit, v2t'));
  (v1', v2');
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