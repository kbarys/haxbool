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
    players->List.keep(player => player.actions.hit && distanceToBall(player, ball) < 0.07)->nearestToBall(ball)
  ) {
  | Some(hittingPlayer) =>
    let ballVelocityAfterHit =
      Vector.adjustLength(
        Vector.subtract(ball.circle.position, hittingPlayer.physicalObject.circle.position),
        ~length=Options.ballVelocityAfterHit,
      );
    {...ball, velocity: ballVelocityAfterHit};
  | None => ball
  };
};