open Jest;

let floatsToFixedStrings = ((a, b, c)) => {
  let f = x => Js.Float.toFixedWithPrecision(x, ~digits=8);
  (f(a), f(b), f(c));
};

describe("Vector.straightEquationCoefficients", () => {
  Expect.(
    test("Vertical straight (x = 2) - (2.0, 5.0), (2.0, 13.0)", () =>
      expect(Vector.straightEquationCoefficients((2.0, 5.0), (2.0, 13.0)))
      |> toEqual((1.0, 0.0, (-2.0)))
    )
  );
  Expect.(
    test("Horizontal straight (y = 4) - (2.0, 4.0), (8.0, 4.0)", () =>
      expect(Vector.straightEquationCoefficients((2.0, 4.0), (8.0, 4.0)))
      |> toEqual((0.0, 1.0, (-4.0)))
    )
  );
  Expect.(
    test("Straight (2x + 7y - 13 = 0) - (4.0, 5.0/.7.0), (5.0, 3.0/.7.0)", () =>
      expect(
        Vector.straightEquationCoefficients(
          (4.0, 5.0 /. 7.0),
          (5.0, 3.0 /. 7.0),
        )
        ->floatsToFixedStrings,
      )
      |> toEqual((2.0 /. 7.0, 1.0, (-13.0) /. 7.0)->floatsToFixedStrings)
    )
  );
});

describe("Vector.distance", () => {
  Expect.(
    test("Distance from (0.1, 0.4) to (0.7, 0.13)", () =>
      expect(Vector.distance((0.1, 0.4), (0.7, 1.3)))
      |> toBeCloseTo(1.081665)
    )
  )
});