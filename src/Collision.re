open Belt;

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

let updatePositions = (objects: List.t(PhysicalObject.t), time) => {
  let initialShiftByObjectId =
    objects
    ->List.map(object_ => (object_.id, Vector.multiplyByScalar(object_.velocity, time)))
    ->List.toArray
    ->Map.String.fromArray;
  let shiftByObjectId =
    makePairs(objects)
    ->List.reduce(
        initialShiftByObjectId,
        (currentShifts, (objectA, objectB)) => {
          let circleA = objectA.circle;
          let circleB = objectB.circle;
          if (Circle.(
                contact(
                  move(circleA, ~by=Map.String.getExn(initialShiftByObjectId, objectA.id)),
                  move(circleB, ~by=Map.String.getExn(initialShiftByObjectId, objectB.id)),
                  ~precission=0.00000000001,
                )
              )) {
            let k =
              circleA.radius
              +. circleB.radius
              /. Vector.length(
                   Vector.add(
                     Vector.multiplyByScalar(Vector.subtract(circleA.position, circleB.position), 2.0),
                     Vector.subtract(
                       Vector.multiplyByScalar(objectA.velocity, time),
                       Vector.multiplyByScalar(objectB.velocity, time),
                     ),
                   ),
                 );
            Js.log2("k = ", k);
            let shiftA = Vector.multiplyByScalar(objectA.velocity, time *. k);
            let shiftB = Vector.multiplyByScalar(objectB.velocity, time *. k);
            let updateShift = (shifts, objectId, computedShift) =>
              Belt.Map.String.update(
                shifts,
                objectId,
                Option.map(_, currentShift =>
                  Vector.length(currentShift) > Vector.length(computedShift) ? computedShift : currentShift
                ),
              );
            currentShifts->updateShift(objectA.id, shiftA)->updateShift(objectB.id, shiftB);
          } else {
            currentShifts;
          };
        },
      );
  List.map(objects, object_ =>
    {
      ...object_,
      circle: {
        ...object_.circle,
        position: Vector.add(object_.circle.position, Map.String.getExn(shiftByObjectId, object_.id)),
      },
    }
  );
};

let velocitiesAfterElasticCollision = (objectA: PhysicalObject.t, objectB: PhysicalObject.t) => {
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

let findCollisions = (objects: Belt.List.t(PhysicalObject.t)) => {
  let foundCollisions =
    objects->makePairs->Belt.List.keep(((a, b)) => Circle.contact(a.circle, b.circle, ~precission=0.00000000001));
  // Js.log2("found collisions", foundCollisions);
  foundCollisions;
};

let toObjectsWithVelocitiesAfterCollision = collisions =>
  collisions
  ->Belt.List.map(((objectA, objectB)) => {
      let (velocityA, velocityB) = velocitiesAfterElasticCollision(objectA, objectB);
      [(objectA, velocityA), (objectB, velocityB)];
    })
  ->Belt.List.flatten;

let toObjectsWithResultantVelocities = (objectsWithVelocities: Belt.List.t((PhysicalObject.t, Vector.t))) =>
  objectsWithVelocities
  ->Belt.List.reduce(Belt.Map.String.empty, (acc, (previuosPhysicalObject, velocity)) =>
      acc->Belt.Map.String.update(
        previuosPhysicalObject.id,
        fun
        | Some(currentPhysicalObject) =>
          Some(
            currentPhysicalObject->PhysicalObject.setVelocity(Vector.add(currentPhysicalObject.velocity, velocity)),
          )
        | None => Some(previuosPhysicalObject->PhysicalObject.setVelocity(velocity)),
      )
    )
  ->Belt.Map.String.valuesToArray
  ->Belt.List.fromArray;

let simulateElasticCollisions = objects => {
  let afterCollision =
    objects->findCollisions->toObjectsWithVelocitiesAfterCollision->toObjectsWithResultantVelocities;
  objects->List.map(object_ =>
    List.getBy(afterCollision, o => o.id == object_.id)
    ->Option.getWithDefault(List.getBy(objects, o => o.id == object_.id)->Option.getExn)
  );
};

let updateObjects = (objects, time) => {
  List.map(objects, PhysicalObject.update(_, time));
};

let findTimeWithCorrectCollision = (objectA, objectB, time) => {
  let {PhysicalObject.circle: circleA} = objectA;
  let {PhysicalObject.circle: circleB} = objectB;
  let decreaseRatio =
    circleA.radius
    +. circleB.radius
    /. Vector.length(
         Vector.add(
           Vector.multiplyByScalar(Vector.subtract(circleA.position, circleB.position), 2.0),
           Vector.subtract(
             Vector.multiplyByScalar(objectA.velocity, time),
             Vector.multiplyByScalar(objectB.velocity, time),
           ),
         ),
       );
  time *. decreaseRatio;
};

let rec update = (objects, time) => {
  let reguralyUpdatedObjects = updateObjects(objects, time);
  let collisions = findCollisions(reguralyUpdatedObjects);
  if (collisions == []) {
    reguralyUpdatedObjects;
  } else {
    let decreasedTime =
      List.map(collisions, ((objectA, objectB)) => findTimeWithCorrectCollision(objectA, objectB, time))
      ->List.toArray
      ->Js.Math.minMany_float;
    update(objects->updateObjects(decreasedTime)->simulateElasticCollisions, time -. decreasedTime);
  };
};