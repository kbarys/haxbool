open Jest;

describe("Circle.collide", () => {
  let circleA: Circle.t = {position: (0.0, 0.0), radius: 5.0};
  let circleB: Circle.t = {position: (0.0, 8.0), radius: 3.0};
  let circleC: Circle.t = {position: (0.0, 12.0), radius: 4.0};
  Expect.(
    test("Cricles collide by the contact", () =>
      expect(Circle.collide(circleA, circleB)) |> toBe(true)
    )
  );
  Expect.(
    test("Cricles don't collide", () =>
      expect(Circle.collide(circleA, circleC)) |> toBe(false)
    )
  );
  Expect.(
    test("Cricles collide by including", () =>
      expect(Circle.collide(circleB, circleC)) |> toBe(true)
    )
  );
});