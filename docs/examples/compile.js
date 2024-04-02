var fs = require('fs');
let data = fs.readFileSync("./model.json");

var kp = require('../../index.js');
var parser = new kp.PopJSON();
var out = parser.parse_json(data);
if (out["error"]) {
    console.log("/* *** ERROR *** */")
    console.log(out["error"])
    console.log("/* ************* */")
}
console.log(out["model"]);
