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
  time *. decreaseRatio;
};

let findTimeWithCorrectCollisions = (collisions: List.t(Collision.t), time) => {
  List.map(collisions, ((objectA, objectB)) => findTimeWithCorrectCollision(objectA, objectB, time))
  ->List.toArray
  ->Js.Math.minMany_float;
};

let rec update = (objectById, time) => {
  let updatedObjects = updateObjectPositionsAndVelocities(objectById, time);
  let collisions = Collision.findCollisions(updatedObjects, ~collisionDetector=Circle.collide);
  if (collisions == []) {
    updatedObjects;
  } else {
    let hypotheticalCollisions =
      List.map(collisions, (({PhysicalObject.id: idA}, {id: idB})) =>
        (Map.String.getExn(objectById, idA), Map.String.getExn(objectById, idB))
      );
    let decreasedTime = findTimeWithCorrectCollisions(hypotheticalCollisions, time);
    update(
      objectById->updateObjectPositionsAndVelocities(decreasedTime)->Collision.withVelocitiesAfterCollisions,
      time -. decreasedTime,
    );
  };
};