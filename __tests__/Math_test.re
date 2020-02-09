open Jest;

let floatsToFixedStrings =
  Belt.List.map(_, Js.Float.toFixedWithPrecision(_, ~digits=8));

describe("Math.solveQuadraticEquation", () => {
  Expect.(
    test("No solution: 4x^2 - 12x + 13 = 0 [(-2x + 3)^2 = -4]", () =>
      expect(Math.solveQuadraticEquation((4.0, (-12.0), 13.0)))
      |> toEqual([])
    )
  );
  Expect.(
    test("One solution: 49x^2 - 28x + 4 = 0 [(7x + 2)^2 = 0]", () =>
      expect(
        Math.solveQuadraticEquation((49.0, 28.0, 4.0))->floatsToFixedStrings,
      )
      |> toEqual([(-2.0) /. 7.0]->floatsToFixedStrings)
    )
  );
  Expect.(
    test("Two solutions: 9x^2 - 6x - 34 = 0 [(3x - 1)^2 = 35]", () =>
      expect(
        Math.solveQuadraticEquation((9.0, (-6.0), (-34.0)))
        ->floatsToFixedStrings,
      )
      |> toEqual(
           [
             (Js.Math.sqrt(35.0) +. 1.0) /. 3.0,
             (-. Js.Math.sqrt(35.0) +. 1.0) /. 3.0,
           ]
           ->floatsToFixedStrings,
         )
    )
  );
});