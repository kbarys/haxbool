open Belt;

type state = {
  idOfControlledPlayer: string,
  players: Belt.List.t(Player.t),
  ball: PhysicalObject.t,
};

let initState = {
  idOfControlledPlayer: "player_1",
  players: [
    Player.create(~id="player_1", ~position=(0.25, 0.5)),
    Player.create(~id="player_2", ~position=(1.0, 0.5)),
  ],
  ball: {
    id: "ball",
    circle: {
      position: (Options.virtualWidth /. 4.0, Options.virtualHeight /. 2.0),
      radius: Options.ballRadius,
    },
    mass: Options.ballMass,
    force: Vector.zero,
    velocity: Vector.zero,
    frictionCoefficient: Options.ballFrictionCoefficient,
  },
};

let nextState = (state, time) => {
  let objectById =
    [state.ball, ...state.players->List.map(({physicalObject}) => physicalObject)]
    ->List.map(object_ => (object_.id, object_))
    ->List.toArray
    ->Map.String.fromArray;
  let updatedObjects = Physics.update(objectById, time);
  let players =
    state.players
    ->Belt.List.map(player =>
        {
          ...player,
          physicalObject: Map.String.getExn(updatedObjects, player.id),
          actions: player.id == state.idOfControlledPlayer ? Input.actions^ : player.actions,
        }
        ->Player.updateForce
        ->Hitting.updatePlayerHit(_, time)
      );
  let ball = Map.String.getExn(updatedObjects, state.ball.id);
  {...state, players, ball: Hitting.updateBallVelocity(players, ball)};
};