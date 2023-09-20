var kp = require('../../keparser.js');
var parser = new kp.PopJSON();
var out = parser.parse_file("./model.json");
if (out["error"]) {
    console.log("/* *** ERROR *** */")
    console.log(out["error"])
    console.log("/* ************* */")
}
console.log(out["model"]);
