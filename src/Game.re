open Belt;

type state = {
  idOfControlledPlayer: string,
  players: Belt.List.t(Player.t),
  ball: PhysicalObject.t,
};

let initState = {
  idOfControlledPlayer: "player_1",
  players: [Player.create(~id="player_1", ~position=(0.25, 0.5))],
  ball: {
    id: "ball",
    circle: {
      position: (Options.virtualWidth /. 4.0, Options.virtualHeight /. 2.0),
      radius: Options.ballRadius,
    },
    mass: Options.ballMass,
    force: Vector.zero,
    velocity: Vector.zero,
    frictionCoefficient: 0.05,
  },
};

let updateControlledPlayerActions = (players: List.t(Player.t), idOfControlledPlayer) => {
  Belt.List.map(
    players,
    fun
    | {physicalObject: {id}} as player when id == idOfControlledPlayer => {...player, actions: Input.actions^}
    | player => player,
  );
};

let nextState = (state, time) => {
  let playersWithUpdatedForces =
    state.players->updateControlledPlayerActions(state.idOfControlledPlayer)->List.map(Player.updateForce);
  let objectById =
    [state.ball, ...playersWithUpdatedForces->List.map(({physicalObject}) => physicalObject)]
    ->List.map(object_ => (object_.id, object_))
    ->List.toArray
    ->Map.String.fromArray;
  let updatedObjects = Physics.update(objectById, time);
  let players =
    state.players
    ->Belt.List.map(player =>
        {...player, physicalObject: Map.String.getExn(updatedObjects, player.physicalObject.id)}
      );
  let ball = Map.String.getExn(updatedObjects, state.ball.id);
  {...state, players, ball};
};