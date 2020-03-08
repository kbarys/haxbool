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
  switch (
    players->List.keep(player => player.hitActivated && distanceToBall(player, ball) < 0.07)->nearestToBall(ball)
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
        player.hitPower == 0.0 ? 1.0 : Js.Math.min_float(player.hitPower +. time *. 5.0, 3.0);
      } else if (hitActivated) {
        player.hitPower;
      } else {
        0.0;
      },
  };
};