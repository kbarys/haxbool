open Belt;

let distanceToBall = (player: Player.t, ball: PhysicalObject.t) =>
  Vector.distance(player.physicalObject.circle.position, ball.circle.position);

let nearestToBall = (players: List.t(Player.t), ball: PhysicalObject.t) => {
  switch (players) {
  | [firstPlayer, ...remainingPlayers] =>
    Some(
      List.reduce(remainingPlayers, firstPlayer, (player, currentlyNearestPlayer) =>
        if (distanceToBall(currentlyNearestPlayer, ball) > distanceToBall(player, ball)) {
          player;
        } else {
          currentlyNearestPlayer;
        }
      ),
    )
  | [] => None
  };
};

let updateBallVelocity = (players: List.t(Player.t), ball: PhysicalObject.t) => {
  let hitprecision = Js.Math.pow_float(~base=10.0, ~exp=-2.0);
  switch (
    players
    ->List.keep(player =>
        player.hitActivated
        && Math.approxEqual(
             player.physicalObject.circle.radius +. ball.circle.radius,
             distanceToBall(player, ball),
             ~precision=hitprecision,
           )
      )
    ->nearestToBall(ball)
  ) {
  | Some(hittingPlayer) =>
    let ballVelocityAfterHit =
      Vector.adjustLength(
        Vector.subtract(ball.circle.position, hittingPlayer.physicalObject.circle.position),
        ~length=Options.hitCoefficient *. hittingPlayer.hitPower,
      );
    {...ball, velocity: ballVelocityAfterHit};
  | None => ball
  };
};

let updatePlayerHit = (player: Player.t, time) => {
  let hitActivated = player.hitPower > 0.0 && !player.actions.hit && !player.hitActivated;
  {
    ...player,
    hitActivated,
    hitPower:
      if (player.actions.hit) {
        Js.Math.min_float(player.hitPower +. time *. 2.0, 1.0);
      } else if (hitActivated) {
        player.hitPower;
      } else {
        0.0;
      },
  };
};