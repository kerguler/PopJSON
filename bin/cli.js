#!/usr/bin/env node

const fs = require('fs');
const { execSync } = require('child_process');
const { Command } = require('commander');
const { PopJSON, version, version_pop } = require('../lib/index.js');

// --- Handle -v/--version BEFORE Commander does anything ---
const argv = process.argv.slice(2);
if (argv.includes('--version') || argv.includes('-v')) {
  console.log(`PopJSON=${version}`);
  console.log(`Population=${version_pop}`);
  process.exit(0);
}

const program = new Command();

let filename = './model';  // default

program
  .name('popjson')
  .description('Translate PopJSON models to C and compile to a dynamic library')
  .arguments('[model]')
  .option('-L, --lib <path>', 'Path to population library')
  .option('-I, --include <path>', 'Path to population headers')
  .option('--no-compile', 'Skip compilation step (only generate C code)')
  .option('--verbose', 'Show extra debug output')
  .option('-v, --version', 'Show PopJSON and population library versions');

program.parse(process.argv);
const options = program.opts();

const filejson = `${filename}.json`;
const filec = `${filename}.c`;
const filedylib = `${filename}.dylib`;

if (options.verbose) {
  console.log(`[popjson] Input JSON: ${filejson}`);
  console.log(`[popjson] C output file: ${filec}`);
  console.log(`[popjson] Dynamic library: ${filedylib}`);
}

if (program.args.length > 0) {
  filename = program.args[0];
}

// --- Read JSON ---
let data;
try {
  data = fs.readFileSync(filejson, 'utf8');
} catch (err) {
  if (program.args.length === 0 && filename === './model') {
    console.error('Error: No model specified and default ./model.json not found.');
    console.error('Usage: popjson <model>   (expects <model>.json to exist)');
  } else {
    console.error(`Error: Could not read ${filejson}`);
  }
  process.exit(1);
}

// --- Parse with PopJSON ---
const parser = new PopJSON();
const out = parser.parse_json(data);

if (out.error) {
  console.error('/* *** ERROR *** */');
  console.error(out.error);
  console.error('/* ************* */');
  process.exit(1);
}

// --- Write C file ---
try {
  fs.writeFileSync(filec, out.model);
  console.log(`Model translated to ${filec}`);
  if (options.verbose) {
    console.log('--- C code preview ---');
    const preview = out.model.split('\n').slice(0, 20).join('\n');
    console.log(preview);
    if (out.model.split('\n').length > 20) {
      console.log('... (truncated)');
    }
    console.log('----------------------');
  }
} catch (err) {
  console.error('Error writing C file:', err.message);
  process.exit(1);
}

// --- Skip compilation if requested ---
if (!options.compile) {
  console.log('Skipping compilation (--no-compile)');
  process.exit(0);
}

// --- Build GCC command dynamically ---
let cmd = `gcc ${filec}`;

let pkgFlags = '';
if (!options.lib && !options.include) {
  try {
    pkgFlags = execSync('pkg-config --cflags --libs population', {
      encoding: 'utf8',
      stdio: ['pipe', 'pipe', 'pipe']  // capture stderr too
    }).trim();

    if (pkgFlags) {
      if (options.verbose) {
        console.log(`[popjson] Using pkg-config flags: ${pkgFlags}`);
      }
    } else {
      if (options.verbose) {
        console.log('[popjson] pkg-config returned no flags for population');
      }
      pkgFlags = ''; // ensure it's empty
    }
  } catch (err) {
    if (options.verbose) {
      console.log('[popjson] pkg-config failed (population.pc not found in search path)');
      console.log('[popjson] You may need to set PKG_CONFIG_PATH, e.g.:');
      console.log('         export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH');
    }
    pkgFlags = ''; // safe fallback
  }
}

if (options.lib) cmd += ` -L${options.lib}`;
if (options.include) cmd += ` -I${options.include}`;
if (pkgFlags) cmd += ` ${pkgFlags}`;

cmd += ' -w -lm -lpopulation -lgsl -lgslcblas -shared -fPIC';
cmd += ` -o ${filedylib}`;

try {
  if (options.verbose) {
    console.log(`[popjson] Running gcc with: ${cmd}`);
  }
  execSync(cmd, { stdio: 'inherit' });
  console.log(`Model file created: ${filedylib}`);
} catch (err) {
  console.error('Compilation failed:', err.message);
  process.exit(1);
}
