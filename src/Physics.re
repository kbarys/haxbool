open Belt;

let updateObjects = (objectById, time) => {
  Map.String.map(objectById, PhysicalObject.update(_, time));
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

let findTimeWithCorrectCollisions = (collisions: List.t(Collision.t), time) => {
  List.map(collisions, ((objectA, objectB)) => findTimeWithCorrectCollision(objectA, objectB, time))
  ->List.toArray
  ->Js.Math.minMany_float;
};

let rec update = (objectById, time) => {
  let updatedObjects = updateObjects(objectById, time);
  let collisions = Collision.findCollisions(updatedObjects);
  if (collisions == []) {
    updatedObjects;
  } else {
    let decreasedTime = findTimeWithCorrectCollisions(collisions, time);
    update(
      objectById->updateObjects(decreasedTime)->Collision.withVelocitiesAfterCollisions,
      time -. decreasedTime,
    );
  };
};