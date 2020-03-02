open Jest;

let floatsToFixedStrings = ((a, b, c)) => {
  let f = x => Js.Float.toFixedWithPrecision(x, ~digits=8);
  (f(a), f(b), f(c));
};

describe("Vector.distance", () => {
  Expect.(
    test("Distance from (0.1, 0.4) to (0.7, 0.13)", () =>
      expect(Vector.distance((0.1, 0.4), (0.7, 1.3))) |> toBeCloseTo(1.081665)
    )
  )
});