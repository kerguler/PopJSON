#!/usr/bin/env node

const pkg = require("@cortex-js/compute-engine");
const ce = new pkg.ComputeEngine();

expr = ce.parse('2^{11}-1 \\in \\P');

console.log(expr.json);
// ➔ ["Element", ["Subtract", ["Power", 2, 11] , 1], "PrimeNumber"]

console.log(expr.latex);

console.log(expr.N());
// ➔ "False"

expr = ce.box(["Expand", ["Power", ["Add", "a", "b"], 2]]);

console.log(expr.latex);

expr = ce.parse("\\mathrm{Expand}((a+b)^2)");

console.log(expr.json);

expr = ce.box(["Expand", ["Power", ["Add", "a", "b"], 2]]);

console.log(expr.latex);
