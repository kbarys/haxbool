open Jest;

describe("Collision.makePairs", () => {
  Expect.(
    test("Pairs of [1, 5, 2, 4, 3] has the correct length", () =>
      expect(Collision.makePairs([1, 5, 2, 4, 3])->Belt.List.toArray)
      |> toHaveLength(10)
    )
  );
  Expect.(
    test("Pairs of [1, 5, 2, 4, 3] contains correct values", () =>
      expect(Collision.makePairs([1, 5, 2, 4, 3])->Belt.List.toArray)
      |> toBeSupersetOf([|
           (1, 5),
           (1, 2),
           (1, 4),
           (1, 3),
           (5, 2),
           (5, 4),
           (5, 3),
           (2, 4),
           (2, 3),
           (4, 3),
         |])
    )
  );
});
