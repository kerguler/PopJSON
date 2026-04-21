const fs = require('fs');
const { PopJSON } = require('../../lib/index.js');

const parser = new PopJSON();
const json = JSON.parse(fs.readFileSync('ex8b.json', 'utf8'));
const out = parser.process_json(json);

if (out.error) {
  console.error(out.error);
} else {
  fs.writeFileSync('ex8b.c', out.model);
}
