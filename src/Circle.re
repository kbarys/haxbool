type t = {
  position: Vector.t,
  radius: float,
};

let setPosition = (circle, newPosition) => {
  ...circle,
  position: newPosition,
};

let collide = (a, b) => {
  let distanceBetweenCenters = Vector.distance(a.position, b.position);
  distanceBetweenCenters <= a.radius +. b.radius;
};

let contactPoint = (a, b) =>
  if (collide(a, b)) {
    let straightThroughCenters =
      Vector.straightEquationCoefficients(a.position, b.position);
    let (sA, sB, sC) = straightThroughCenters;
    let takeCloser = (c1, c2) =>
      Vector.distance(c1, b.position) < Vector.distance(c2, b.position)
        ? c1 : c2;
    if (sA == 0.0) {
      let c1 = Vector.add(a.position, (a.radius, 0.0));
      let c2 = Vector.add(a.position, (-. a.radius, 0.0));
      Some(takeCloser(c1, c2));
    } else if (sB == 0.0) {
      let c1 = Vector.add(a.position, (0.0, a.radius));
      let c2 = Vector.add(a.position, (0.0, -. a.radius));
      Some(takeCloser(c1, c2));
    } else {
      let (cx, cy) = a.position;
      let quad = Js.Math.pow_float(~base=_, ~exp=2.0);
      let y = x => -. (sA *. x +. sC) /. sB;
      switch (
        Math.solveQuadraticEquation((
          1.0 +. quad(sA /. sB),
          2.0 *. sA /. sB *. (cy +. sC /. sB) -. 2.0 *. cx,
          quad(cx)
          +. quad(cy)
          +. 2.0
          *. cy
          *. (sC /. sB)
          +. quad(sC /. sB)
          -. quad(a.radius),
        ))
      ) {
      | [x1, x2] =>
        let c1 = (x1, y(x1));
        let c2 = (x2, y(x2));
        Some(takeCloser(c1, c2));
      | _ =>
        None;
      };
    };
  } else {
    None;
  };