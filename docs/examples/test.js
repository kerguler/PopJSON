const fs = require('fs');
const { PopJSON } = require('../../lib/index.js');

const parser = new PopJSON();
const json = JSON.parse(fs.readFileSync('ex10.json', 'utf8'));
const out = parser.process_json(json);

if (out.error) {
  console.error(out.error);
} else {
  fs.writeFileSync('ex10.c', out.model);
}
