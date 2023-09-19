var kp = require('../../keparser.js');
var parser = new kp.PopJSON();
console.log((parser.parse_file("./model.json")["model"]));
