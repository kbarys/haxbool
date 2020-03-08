open Belt;

let updateObjectPositionsAndVelocities = (objectById, time) => {
  Map.String.map(objectById, PhysicalObject.updatePositionAndVelocity(_, time));
};

let findTimeWithCorrectCollision = (objectA, objectB, time) => {
  let {PhysicalObject.circle: circleA} = objectA;
  let {PhysicalObject.circle: circleB} = objectB;
  let (t1, t2) =
    Vector.(subtract(multiplyByScalar(objectA.velocity, time), multiplyByScalar(objectB.velocity, time)));
  let (c1, c2) = Vector.subtract(circleA.position, circleB.position);
  let j = Math.quad(t1) +. Math.quad(t2);
  let i = 2.0 *. (t1 *. c1 +. t2 *. c2);
  let z = Math.quad(c1) +. Math.quad(c2) -. Math.quad(circleA.radius +. circleB.radius);
  let decreaseRatio = Math.solveQuadraticEquation((j, i, z))->List.toArray->Js.Math.minMany_float;
  let decreasedTime = time *. decreaseRatio;
  decreasedTime;
};

let findTimeWithCorrectCollisions = (collidingObjects: List.t((PhysicalObject.t, PhysicalObject.t)), time) => {
  List.map(collidingObjects, ((objectA, objectB)) => findTimeWithCorrectCollision(objectA, objectB, time))
  ->List.toArray
  ->Js.Math.minMany_float;
};

let rec update = (objectById, time) => {
  let updatedObjects = updateObjectPositionsAndVelocities(objectById, time);
  let hypotheticalCollisions = Collision.findCollisions(updatedObjects);
  if (hypotheticalCollisions == []) {
    updatedObjects;
  } else {
    let decreasedTime =
      findTimeWithCorrectCollisions(
        hypotheticalCollisions->List.map(Collision.collidingObjectsFromCollision(objectById)),
        time,
      );
    let objectByIdAfterDecreasedTime = objectById->updateObjectPositionsAndVelocities(decreasedTime);
    update(
      objectByIdAfterDecreasedTime->Collision.withVelocitiesAfterCollisions(hypotheticalCollisions),
      time -. decreasedTime,
    );
  };
};