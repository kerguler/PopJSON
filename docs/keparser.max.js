require=(function(){function r(e,n,t){function o(i,f){if(!n[i]){if(!e[i]){var c="function"==typeof require&&require;if(!f&&c)return c(i,!0);if(u)return u(i,!0);var a=new Error("Cannot find module '"+i+"'");throw a.code="MODULE_NOT_FOUND",a}var p=n[i]={exports:{}};e[i][0].call(p.exports,function(r){var n=e[i][1][r];return o(n||r)},p,p.exports,r,e,n,t)}return n[i].exports}for(var u="function"==typeof require&&require,i=0;i<t.length;i++)o(t[i]);return o}return r})()({1:[function(require,module,exports){

},{}],2:[function(require,module,exports){
if (typeof Object.create === 'function') {
  // implementation from standard node.js 'util' module
  module.exports = function inherits(ctor, superCtor) {
    ctor.super_ = superCtor
    ctor.prototype = Object.create(superCtor.prototype, {
      constructor: {
        value: ctor,
        enumerable: false,
        writable: true,
        configurable: true
      }
    });
  };
} else {
  // old school shim for old browsers
  module.exports = function inherits(ctor, superCtor) {
    ctor.super_ = superCtor
    var TempCtor = function () {}
    TempCtor.prototype = superCtor.prototype
    ctor.prototype = new TempCtor()
    ctor.prototype.constructor = ctor
  }
}

},{}],3:[function(require,module,exports){
// shim for using process in browser
var process = module.exports = {};

// cached from whatever global is present so that test runners that stub it
// don't break things.  But we need to wrap it in a try catch in case it is
// wrapped in strict mode code which doesn't define any globals.  It's inside a
// function because try/catches deoptimize in certain engines.

var cachedSetTimeout;
var cachedClearTimeout;

function defaultSetTimout() {
    throw new Error('setTimeout has not been defined');
}
function defaultClearTimeout () {
    throw new Error('clearTimeout has not been defined');
}
(function () {
    try {
        if (typeof setTimeout === 'function') {
            cachedSetTimeout = setTimeout;
        } else {
            cachedSetTimeout = defaultSetTimout;
        }
    } catch (e) {
        cachedSetTimeout = defaultSetTimout;
    }
    try {
        if (typeof clearTimeout === 'function') {
            cachedClearTimeout = clearTimeout;
        } else {
            cachedClearTimeout = defaultClearTimeout;
        }
    } catch (e) {
        cachedClearTimeout = defaultClearTimeout;
    }
} ())
function runTimeout(fun) {
    if (cachedSetTimeout === setTimeout) {
        //normal enviroments in sane situations
        return setTimeout(fun, 0);
    }
    // if setTimeout wasn't available but was latter defined
    if ((cachedSetTimeout === defaultSetTimout || !cachedSetTimeout) && setTimeout) {
        cachedSetTimeout = setTimeout;
        return setTimeout(fun, 0);
    }
    try {
        // when when somebody has screwed with setTimeout but no I.E. maddness
        return cachedSetTimeout(fun, 0);
    } catch(e){
        try {
            // When we are in I.E. but the script has been evaled so I.E. doesn't trust the global object when called normally
            return cachedSetTimeout.call(null, fun, 0);
        } catch(e){
            // same as above but when it's a version of I.E. that must have the global object for 'this', hopfully our context correct otherwise it will throw a global error
            return cachedSetTimeout.call(this, fun, 0);
        }
    }


}
function runClearTimeout(marker) {
    if (cachedClearTimeout === clearTimeout) {
        //normal enviroments in sane situations
        return clearTimeout(marker);
    }
    // if clearTimeout wasn't available but was latter defined
    if ((cachedClearTimeout === defaultClearTimeout || !cachedClearTimeout) && clearTimeout) {
        cachedClearTimeout = clearTimeout;
        return clearTimeout(marker);
    }
    try {
        // when when somebody has screwed with setTimeout but no I.E. maddness
        return cachedClearTimeout(marker);
    } catch (e){
        try {
            // When we are in I.E. but the script has been evaled so I.E. doesn't  trust the global object when called normally
            return cachedClearTimeout.call(null, marker);
        } catch (e){
            // same as above but when it's a version of I.E. that must have the global object for 'this', hopfully our context correct otherwise it will throw a global error.
            // Some versions of I.E. have different rules for clearTimeout vs setTimeout
            return cachedClearTimeout.call(this, marker);
        }
    }



}
var queue = [];
var draining = false;
var currentQueue;
var queueIndex = -1;

function cleanUpNextTick() {
    if (!draining || !currentQueue) {
        return;
    }
    draining = false;
    if (currentQueue.length) {
        queue = currentQueue.concat(queue);
    } else {
        queueIndex = -1;
    }
    if (queue.length) {
        drainQueue();
    }
}

function drainQueue() {
    if (draining) {
        return;
    }
    var timeout = runTimeout(cleanUpNextTick);
    draining = true;

    var len = queue.length;
    while(len) {
        currentQueue = queue;
        queue = [];
        while (++queueIndex < len) {
            if (currentQueue) {
                currentQueue[queueIndex].run();
            }
        }
        queueIndex = -1;
        len = queue.length;
    }
    currentQueue = null;
    draining = false;
    runClearTimeout(timeout);
}

process.nextTick = function (fun) {
    var args = new Array(arguments.length - 1);
    if (arguments.length > 1) {
        for (var i = 1; i < arguments.length; i++) {
            args[i - 1] = arguments[i];
        }
    }
    queue.push(new Item(fun, args));
    if (queue.length === 1 && !draining) {
        runTimeout(drainQueue);
    }
};

// v8 likes predictible objects
function Item(fun, array) {
    this.fun = fun;
    this.array = array;
}
Item.prototype.run = function () {
    this.fun.apply(null, this.array);
};
process.title = 'browser';
process.browser = true;
process.env = {};
process.argv = [];
process.version = ''; // empty string to avoid regexp issues
process.versions = {};

function noop() {}

process.on = noop;
process.addListener = noop;
process.once = noop;
process.off = noop;
process.removeListener = noop;
process.removeAllListeners = noop;
process.emit = noop;
process.prependListener = noop;
process.prependOnceListener = noop;

process.listeners = function (name) { return [] }

process.binding = function (name) {
    throw new Error('process.binding is not supported');
};

process.cwd = function () { return '/' };
process.chdir = function (dir) {
    throw new Error('process.chdir is not supported');
};
process.umask = function() { return 0; };

},{}],4:[function(require,module,exports){
module.exports = function isBuffer(arg) {
  return arg && typeof arg === 'object'
    && typeof arg.copy === 'function'
    && typeof arg.fill === 'function'
    && typeof arg.readUInt8 === 'function';
}
},{}],5:[function(require,module,exports){
(function (process,global){
// Copyright Joyent, Inc. and other Node contributors.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
// USE OR OTHER DEALINGS IN THE SOFTWARE.

var formatRegExp = /%[sdj%]/g;
exports.format = function(f) {
  if (!isString(f)) {
    var objects = [];
    for (var i = 0; i < arguments.length; i++) {
      objects.push(inspect(arguments[i]));
    }
    return objects.join(' ');
  }

  var i = 1;
  var args = arguments;
  var len = args.length;
  var str = String(f).replace(formatRegExp, function(x) {
    if (x === '%%') return '%';
    if (i >= len) return x;
    switch (x) {
      case '%s': return String(args[i++]);
      case '%d': return Number(args[i++]);
      case '%j':
        try {
          return JSON.stringify(args[i++]);
        } catch (_) {
          return '[Circular]';
        }
      default:
        return x;
    }
  });
  for (var x = args[i]; i < len; x = args[++i]) {
    if (isNull(x) || !isObject(x)) {
      str += ' ' + x;
    } else {
      str += ' ' + inspect(x);
    }
  }
  return str;
};


// Mark that a method should not be used.
// Returns a modified function which warns once by default.
// If --no-deprecation is set, then it is a no-op.
exports.deprecate = function(fn, msg) {
  // Allow for deprecating things in the process of starting up.
  if (isUndefined(global.process)) {
    return function() {
      return exports.deprecate(fn, msg).apply(this, arguments);
    };
  }

  if (process.noDeprecation === true) {
    return fn;
  }

  var warned = false;
  function deprecated() {
    if (!warned) {
      if (process.throwDeprecation) {
        throw new Error(msg);
      } else if (process.traceDeprecation) {
        console.trace(msg);
      } else {
        console.error(msg);
      }
      warned = true;
    }
    return fn.apply(this, arguments);
  }

  return deprecated;
};


var debugs = {};
var debugEnviron;
exports.debuglog = function(set) {
  if (isUndefined(debugEnviron))
    debugEnviron = process.env.NODE_DEBUG || '';
  set = set.toUpperCase();
  if (!debugs[set]) {
    if (new RegExp('\\b' + set + '\\b', 'i').test(debugEnviron)) {
      var pid = process.pid;
      debugs[set] = function() {
        var msg = exports.format.apply(exports, arguments);
        console.error('%s %d: %s', set, pid, msg);
      };
    } else {
      debugs[set] = function() {};
    }
  }
  return debugs[set];
};


/**
 * Echos the value of a value. Trys to print the value out
 * in the best way possible given the different types.
 *
 * @param {Object} obj The object to print out.
 * @param {Object} opts Optional options object that alters the output.
 */
/* legacy: obj, showHidden, depth, colors*/
function inspect(obj, opts) {
  // default options
  var ctx = {
    seen: [],
    stylize: stylizeNoColor
  };
  // legacy...
  if (arguments.length >= 3) ctx.depth = arguments[2];
  if (arguments.length >= 4) ctx.colors = arguments[3];
  if (isBoolean(opts)) {
    // legacy...
    ctx.showHidden = opts;
  } else if (opts) {
    // got an "options" object
    exports._extend(ctx, opts);
  }
  // set default options
  if (isUndefined(ctx.showHidden)) ctx.showHidden = false;
  if (isUndefined(ctx.depth)) ctx.depth = 2;
  if (isUndefined(ctx.colors)) ctx.colors = false;
  if (isUndefined(ctx.customInspect)) ctx.customInspect = true;
  if (ctx.colors) ctx.stylize = stylizeWithColor;
  return formatValue(ctx, obj, ctx.depth);
}
exports.inspect = inspect;


// http://en.wikipedia.org/wiki/ANSI_escape_code#graphics
inspect.colors = {
  'bold' : [1, 22],
  'italic' : [3, 23],
  'underline' : [4, 24],
  'inverse' : [7, 27],
  'white' : [37, 39],
  'grey' : [90, 39],
  'black' : [30, 39],
  'blue' : [34, 39],
  'cyan' : [36, 39],
  'green' : [32, 39],
  'magenta' : [35, 39],
  'red' : [31, 39],
  'yellow' : [33, 39]
};

// Don't use 'blue' not visible on cmd.exe
inspect.styles = {
  'special': 'cyan',
  'number': 'yellow',
  'boolean': 'yellow',
  'undefined': 'grey',
  'null': 'bold',
  'string': 'green',
  'date': 'magenta',
  // "name": intentionally not styling
  'regexp': 'red'
};


function stylizeWithColor(str, styleType) {
  var style = inspect.styles[styleType];

  if (style) {
    return '\u001b[' + inspect.colors[style][0] + 'm' + str +
           '\u001b[' + inspect.colors[style][1] + 'm';
  } else {
    return str;
  }
}


function stylizeNoColor(str, styleType) {
  return str;
}


function arrayToHash(array) {
  var hash = {};

  array.forEach(function(val, idx) {
    hash[val] = true;
  });

  return hash;
}


function formatValue(ctx, value, recurseTimes) {
  // Provide a hook for user-specified inspect functions.
  // Check that value is an object with an inspect function on it
  if (ctx.customInspect &&
      value &&
      isFunction(value.inspect) &&
      // Filter out the util module, it's inspect function is special
      value.inspect !== exports.inspect &&
      // Also filter out any prototype objects using the circular check.
      !(value.constructor && value.constructor.prototype === value)) {
    var ret = value.inspect(recurseTimes, ctx);
    if (!isString(ret)) {
      ret = formatValue(ctx, ret, recurseTimes);
    }
    return ret;
  }

  // Primitive types cannot have properties
  var primitive = formatPrimitive(ctx, value);
  if (primitive) {
    return primitive;
  }

  // Look up the keys of the object.
  var keys = Object.keys(value);
  var visibleKeys = arrayToHash(keys);

  if (ctx.showHidden) {
    keys = Object.getOwnPropertyNames(value);
  }

  // IE doesn't make error fields non-enumerable
  // http://msdn.microsoft.com/en-us/library/ie/dww52sbt(v=vs.94).aspx
  if (isError(value)
      && (keys.indexOf('message') >= 0 || keys.indexOf('description') >= 0)) {
    return formatError(value);
  }

  // Some type of object without properties can be shortcutted.
  if (keys.length === 0) {
    if (isFunction(value)) {
      var name = value.name ? ': ' + value.name : '';
      return ctx.stylize('[Function' + name + ']', 'special');
    }
    if (isRegExp(value)) {
      return ctx.stylize(RegExp.prototype.toString.call(value), 'regexp');
    }
    if (isDate(value)) {
      return ctx.stylize(Date.prototype.toString.call(value), 'date');
    }
    if (isError(value)) {
      return formatError(value);
    }
  }

  var base = '', array = false, braces = ['{', '}'];

  // Make Array say that they are Array
  if (isArray(value)) {
    array = true;
    braces = ['[', ']'];
  }

  // Make functions say that they are functions
  if (isFunction(value)) {
    var n = value.name ? ': ' + value.name : '';
    base = ' [Function' + n + ']';
  }

  // Make RegExps say that they are RegExps
  if (isRegExp(value)) {
    base = ' ' + RegExp.prototype.toString.call(value);
  }

  // Make dates with properties first say the date
  if (isDate(value)) {
    base = ' ' + Date.prototype.toUTCString.call(value);
  }

  // Make error with message first say the error
  if (isError(value)) {
    base = ' ' + formatError(value);
  }

  if (keys.length === 0 && (!array || value.length == 0)) {
    return braces[0] + base + braces[1];
  }

  if (recurseTimes < 0) {
    if (isRegExp(value)) {
      return ctx.stylize(RegExp.prototype.toString.call(value), 'regexp');
    } else {
      return ctx.stylize('[Object]', 'special');
    }
  }

  ctx.seen.push(value);

  var output;
  if (array) {
    output = formatArray(ctx, value, recurseTimes, visibleKeys, keys);
  } else {
    output = keys.map(function(key) {
      return formatProperty(ctx, value, recurseTimes, visibleKeys, key, array);
    });
  }

  ctx.seen.pop();

  return reduceToSingleString(output, base, braces);
}


function formatPrimitive(ctx, value) {
  if (isUndefined(value))
    return ctx.stylize('undefined', 'undefined');
  if (isString(value)) {
    var simple = '\'' + JSON.stringify(value).replace(/^"|"$/g, '')
                                             .replace(/'/g, "\\'")
                                             .replace(/\\"/g, '"') + '\'';
    return ctx.stylize(simple, 'string');
  }
  if (isNumber(value))
    return ctx.stylize('' + value, 'number');
  if (isBoolean(value))
    return ctx.stylize('' + value, 'boolean');
  // For some reason typeof null is "object", so special case here.
  if (isNull(value))
    return ctx.stylize('null', 'null');
}


function formatError(value) {
  return '[' + Error.prototype.toString.call(value) + ']';
}


function formatArray(ctx, value, recurseTimes, visibleKeys, keys) {
  var output = [];
  for (var i = 0, l = value.length; i < l; ++i) {
    if (hasOwnProperty(value, String(i))) {
      output.push(formatProperty(ctx, value, recurseTimes, visibleKeys,
          String(i), true));
    } else {
      output.push('');
    }
  }
  keys.forEach(function(key) {
    if (!key.match(/^\d+$/)) {
      output.push(formatProperty(ctx, value, recurseTimes, visibleKeys,
          key, true));
    }
  });
  return output;
}


function formatProperty(ctx, value, recurseTimes, visibleKeys, key, array) {
  var name, str, desc;
  desc = Object.getOwnPropertyDescriptor(value, key) || { value: value[key] };
  if (desc.get) {
    if (desc.set) {
      str = ctx.stylize('[Getter/Setter]', 'special');
    } else {
      str = ctx.stylize('[Getter]', 'special');
    }
  } else {
    if (desc.set) {
      str = ctx.stylize('[Setter]', 'special');
    }
  }
  if (!hasOwnProperty(visibleKeys, key)) {
    name = '[' + key + ']';
  }
  if (!str) {
    if (ctx.seen.indexOf(desc.value) < 0) {
      if (isNull(recurseTimes)) {
        str = formatValue(ctx, desc.value, null);
      } else {
        str = formatValue(ctx, desc.value, recurseTimes - 1);
      }
      if (str.indexOf('\n') > -1) {
        if (array) {
          str = str.split('\n').map(function(line) {
            return '  ' + line;
          }).join('\n').substr(2);
        } else {
          str = '\n' + str.split('\n').map(function(line) {
            return '   ' + line;
          }).join('\n');
        }
      }
    } else {
      str = ctx.stylize('[Circular]', 'special');
    }
  }
  if (isUndefined(name)) {
    if (array && key.match(/^\d+$/)) {
      return str;
    }
    name = JSON.stringify('' + key);
    if (name.match(/^"([a-zA-Z_][a-zA-Z_0-9]*)"$/)) {
      name = name.substr(1, name.length - 2);
      name = ctx.stylize(name, 'name');
    } else {
      name = name.replace(/'/g, "\\'")
                 .replace(/\\"/g, '"')
                 .replace(/(^"|"$)/g, "'");
      name = ctx.stylize(name, 'string');
    }
  }

  return name + ': ' + str;
}


function reduceToSingleString(output, base, braces) {
  var numLinesEst = 0;
  var length = output.reduce(function(prev, cur) {
    numLinesEst++;
    if (cur.indexOf('\n') >= 0) numLinesEst++;
    return prev + cur.replace(/\u001b\[\d\d?m/g, '').length + 1;
  }, 0);

  if (length > 60) {
    return braces[0] +
           (base === '' ? '' : base + '\n ') +
           ' ' +
           output.join(',\n  ') +
           ' ' +
           braces[1];
  }

  return braces[0] + base + ' ' + output.join(', ') + ' ' + braces[1];
}


// NOTE: These type checking functions intentionally don't use `instanceof`
// because it is fragile and can be easily faked with `Object.create()`.
function isArray(ar) {
  return Array.isArray(ar);
}
exports.isArray = isArray;

function isBoolean(arg) {
  return typeof arg === 'boolean';
}
exports.isBoolean = isBoolean;

function isNull(arg) {
  return arg === null;
}
exports.isNull = isNull;

function isNullOrUndefined(arg) {
  return arg == null;
}
exports.isNullOrUndefined = isNullOrUndefined;

function isNumber(arg) {
  return typeof arg === 'number';
}
exports.isNumber = isNumber;

function isString(arg) {
  return typeof arg === 'string';
}
exports.isString = isString;

function isSymbol(arg) {
  return typeof arg === 'symbol';
}
exports.isSymbol = isSymbol;

function isUndefined(arg) {
  return arg === void 0;
}
exports.isUndefined = isUndefined;

function isRegExp(re) {
  return isObject(re) && objectToString(re) === '[object RegExp]';
}
exports.isRegExp = isRegExp;

function isObject(arg) {
  return typeof arg === 'object' && arg !== null;
}
exports.isObject = isObject;

function isDate(d) {
  return isObject(d) && objectToString(d) === '[object Date]';
}
exports.isDate = isDate;

function isError(e) {
  return isObject(e) &&
      (objectToString(e) === '[object Error]' || e instanceof Error);
}
exports.isError = isError;

function isFunction(arg) {
  return typeof arg === 'function';
}
exports.isFunction = isFunction;

function isPrimitive(arg) {
  return arg === null ||
         typeof arg === 'boolean' ||
         typeof arg === 'number' ||
         typeof arg === 'string' ||
         typeof arg === 'symbol' ||  // ES6 symbol
         typeof arg === 'undefined';
}
exports.isPrimitive = isPrimitive;

exports.isBuffer = require('./support/isBuffer');

function objectToString(o) {
  return Object.prototype.toString.call(o);
}


function pad(n) {
  return n < 10 ? '0' + n.toString(10) : n.toString(10);
}


var months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep',
              'Oct', 'Nov', 'Dec'];

// 26 Feb 16:19:34
function timestamp() {
  var d = new Date();
  var time = [pad(d.getHours()),
              pad(d.getMinutes()),
              pad(d.getSeconds())].join(':');
  return [d.getDate(), months[d.getMonth()], time].join(' ');
}


// log is just a thin wrapper to console.log that prepends a timestamp
exports.log = function() {
  console.log('%s - %s', timestamp(), exports.format.apply(exports, arguments));
};


/**
 * Inherit the prototype methods from one constructor into another.
 *
 * The Function.prototype.inherits from lang.js rewritten as a standalone
 * function (not on Function.prototype). NOTE: If this file is to be loaded
 * during bootstrapping this function needs to be rewritten using some native
 * functions as prototype setup using normal JavaScript does not work as
 * expected during bootstrapping (see mirror.js in r114903).
 *
 * @param {function} ctor Constructor function which needs to inherit the
 *     prototype.
 * @param {function} superCtor Constructor function to inherit prototype from.
 */
exports.inherits = require('inherits');

exports._extend = function(origin, add) {
  // Don't do anything if add isn't an object
  if (!add || !isObject(add)) return origin;

  var keys = Object.keys(add);
  var i = keys.length;
  while (i--) {
    origin[keys[i]] = add[keys[i]];
  }
  return origin;
};

function hasOwnProperty(obj, prop) {
  return Object.prototype.hasOwnProperty.call(obj, prop);
}

}).call(this,require('_process'),typeof global !== "undefined" ? global : typeof self !== "undefined" ? self : typeof window !== "undefined" ? window : {})
},{"./support/isBuffer":4,"_process":3,"inherits":2}],"PopJSON":[function(require,module,exports){
'use strict';

const fs = require('fs');
// const { emitWarning } = require('process');
const util = require('util');

const arbiter = {
    'ACC_FIXED': 'd',
    'ACC_ERLANG': 'd',
    'ACC_PASCAL': 'd',
    'AGE_FIXED': 'i',
    'AGE_CONST': 'i',
    'AGE_GAMMA': 'i',
    'AGE_NBINOM': 'i',
    'AGE_CUSTOM': 'i',
    'NOAGE_CONST': 'i'
};

const stepper = {
    'NO_STEPPER': '0',
    'AGE_STEPPER': 'age_stepper',
    'ACC_STEPPER': 'acc_stepper'
};

const hazard = {
    'AGE_FIXED': ['age_fixed_pars', 'age_hazard_calc', 'age_fixed_haz'],
    'AGE_CONST': ['age_const_pars', 'age_const_calc', 'age_const_haz'],
    'AGE_GAMMA': ['age_gamma_pars','age_hazard_calc','age_gamma_haz'],
    'AGE_NBINOM': ['age_nbinom_pars', 'age_hazard_calc', 'age_nbinom_haz'],
    'NOAGE_CONST': ['age_const_pars', 'age_const_calc', 'age_const_haz']
};

class PopJSON {
    constructor() {
        this.header = "";
        this.model = "";
        this.error = "";
        this.json = {};
    }
    check_ids(id) {
        if (!/^[a-zA-Z][a-zA-Z0-9\_]+$/.test(id))
            this.error += "Only numeric and alphanumeric characters and \"_\" are allowed in IDs. Also, IDs should be more than one character and start with an alphanumeric.\nViolating ID is " + id + "\n";
        return id;
    }
    results() {
        return({
            "model": this.header + this.model,
            "error": this.error
        });
    }
    parse_file(filename) {
        this.filename = filename;
        let data = fs.readFileSync(this.filename);
        if (!data) {
            this.error += "File not found!\n";
            return(this.results());
        }
        try {
            this.json = JSON.parse(data);
        } catch (e) {
            this.error += "JSON parse error!\n";
            return(this.results());
        }
        //
        this.parse();
        return(this.results());
    }
    parse_json(text) {
        try {
            this.json = JSON.parse(text);
        } catch (e) {
            this.error += "JSON parse error!\n";
            return(this.results());
        }
        //
        this.parse();
        return(this.results());
    }
    parse() {
        let that = this;
        this.deterministic = this.json['model']['deterministic'];
        if (!('environ' in this.json)) this.json['environ'] = [];
        this.environs = this.json['environ'].map( (pr) => that.check_ids(pr['id']) );
        if (!('populations' in this.json)) {
            this.json['populations'] = [];
            this.error += "Couldn't find any populations!\n";
            return(this.results());
        }
        this.populations = this.json['populations'].map( (pr) => that.check_ids(pr['id']) );
        this.processes = []; this.json['populations'].forEach( (pop) => 'processes' in pop ? pop['processes'].forEach( (pr) => { that.processes.push(that.check_ids(pr['id'])); } ) : [] );
        this.processobj = {}; this.json['populations'].forEach( (pop) => 'processes' in pop ? pop['processes'].forEach( (pr) => { that.processobj[that.check_ids(pr['id'])] = pr; that.processobj[that.check_ids(pr['id'])]['parent_id'] = that.check_ids(pop['id']); } ) : {} );
        if (!('parameters' in this.json)) this.json['parameters'] = [];
        this.parametersv = this.json['parameters'].filter( (p) => !p['constant'] ).map( (pr) => that.check_ids(pr['id']) );
        this.parametersc = this.json['parameters'].filter( (p) => p['constant'] ).map( (pr) => that.check_ids(pr['id']) );
        if (!('functions' in this.json)) this.json['functions'] = [];
        this.functions = Object.keys(this.json['functions']);
        if (!('intermediates' in this.json)) this.json['intermediates'] = [];
        this.intermediates = this.json['intermediates'].map( (pr) => that.check_ids(pr['id']) );
        if (!('transformations' in this.json)) this.json['transformations'] = [];
        this.transformations = this.json['transformations'].map( (pr) => that.check_ids(pr['id']) );
        if (!('transfers' in this.json)) this.json['transfers'] = [];
        this.transfers = Array.from(new Set(this.json['transfers'].map( (pr) => that.processobj[pr['from']]['parent_id'] )));
        this.operations = ["min","max","round","poisson","binomial","define","?","&&","||",">=","<=",">","<","==","sqrt","pow","exp","log","log2","log10","indicator","index","size","count","*","+","-","/"];
        this.funparnames = ["a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
                            "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
        //
        this.funcountid = 0;
        //
        this.header = "";
        this.model = "";
        if (!this.error)
            this.write_model();
    }
    write_model() {
        this.funcountid = 0;
        //
        this.header = "";
        this.model = "";
        this.write_header();
        this.write_functions();
        this.write_transfer();
        this.write_custom();
        this.write_init();
        this.write_parnames();
        this.write_destroy();
        this.write_sim();
        this.write_main();
    }
    write_header() {
        let that = this;
        if (this.json['model']['type'] == "Population") {
            this.header += "#include <math.h>\n";
            this.header += "#include \"population.h\"\n";
            this.header += "\n";
            if (!this.deterministic) {
                this.header += "extern gsl_rng *RANDOM;\n";
                this.header += "\n";
            } else if (this.json['model']['type'] == "ODE") {
                this.header += "#include <stdlib.h>\n";
                this.header += "#include \"lsoda.h\"\n";
                this.header += "\n";
            }
        }
        //
        this.header += "#define CHECK(x) (isnan(x) || isinf(x))\n"
        this.header += "\n";
        //
        this.numpar = this.json['parameters'].filter( (p) => !p['constant'] ).length;
        this.numpop = this.json['populations'].length;
        this.numint_int = this.json['intermediates'].length;
        this.numint_trans = this.json['transformations'].length;
        this.numint = this.numint_int + this.numint_trans;
        //
        if (this.json['model']['type'] == "Population") {
            if (this.json['populations'].length == 0) {
                this.numproc = 0;
                this.numprocpar = 0;
            } else {
                this.numproc = 1 + Math.max(...this.json['populations'].map( (s) => 'processes' in s ? s['processes'].length : 0 ));
                this.numprocpar = Math.max(...this.json['populations'].map( (spc, i) => {
                    let pars = 0;
                    if (!('processes' in spc)) return pars;
                    spc['processes'].forEach( (s) => {
                        if (Array.isArray(s['value'])) {
                            pars += s['value'].length;
                        } else {
                            pars += 1;
                        }
                    } );
                    return pars;
                } ));
            }
        }
        //
        this.popart = {};
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (pop) => {
                if (!(pop['id'] in that.popart)) { that.popart[pop['id']] = {}; }
                pop['processes'].forEach( (proc) => {
                    that.popart[pop['id']][proc['arbiter']] = arbiter[proc['arbiter']];
                } );
            } );
        }
        //
        this.header += "#define NumPar " + util.format(this.numpar) + "\n";
        this.header += "#define NumPop " + util.format(this.numpop) + "\n";
        this.header += "#define NumInt " + util.format(this.numint) + "\n";
        this.header += "\n";
        //
        this.json['parameters'].filter( (p) => !p['constant'] ).forEach( (pr, i) => {
            that.header += "#define " + pr['id'] + " " + util.format(i) + "\n";
        } );
        this.header += "\n";
        //
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (spc, i) => {
                spc['processes'].forEach( (prc, j) => {
                    that.header += "#define " + prc['id'] + " " + util.format(j) + "\n";
                } );
            } );
            this.header += "\n";
        }
        //
        this.json['parameters'].filter( (p) => p['constant'] ).forEach( (p) => {
            that.header += "double " + p['id'] + " = " + util.format(p['value']) + ";\n";
        } );
        this.header += "\n";
        //
        this.header += "double dmin(double a, double b) { return a < b ? a : b; }\n";
        this.header += "double dmax(double a, double b) { return a > b ? a : b; }\n";
        this.header += "\n";
        //
        this.header += "double *model_param;\n";
        if ('environ' in this.json) {
            this.json['environ'].forEach( (elm, i) => {
                 that.header += "double *envir_" + elm['id'] + ";\n";
            });
            this.header += "\n";
        }
        //
        if ('transformations' in this.json) {
            if (this.deterministic) {
                this.json['transformations'].map( (trx) => trx['id'] ).forEach( (id) => {
                    that.header += "double " + id + ";\n";
                } );
            } else {
                this.json['transformations'].map( (trx) => trx['id'] ).forEach( (id) => {
                    that.header += "unsigned int " + id + ";\n";
                } );
            }
        }
        //
        if ('intermediates' in this.json) {
            this.json['intermediates'].forEach( (elm, i) => {
                that.header += "double " + elm['id'] + ";\n";
            });
            this.header += "\n";
        }
    }
    write_functions() {
        let that = this;
        if ('functions' in this.json) {
           Object.entries(this.json['functions']).forEach( ([key, value]) => {
                that.header += "#define " + key + that.parse_value(value) + "\n";
           });
           this.header += "\n";
        }
    }
    write_transfer() {
        if (!('transfers' in this.json)) return;
        //
        let that = this;
        var di, pop;
        this.json['transfers'].forEach( (trn) => {
            that.header += "void fun_transfer_" + trn['id'] + "(number *key, number num, void *pop) {\n";
            that.header += "    number q[" + util.format(that.numproc) + "] = {\n";
            // pop = that.json['populations'].filter( (tmp) => tmp['id'] == that.processobj[trn['to']]['parent_id'] )[0];
            pop = that.json['populations'].filter( (tmp) => tmp['id'] == trn['to'] )[0];
            pop['processes'].forEach( (proc, j) => {
                di = that.popart[pop['id']][proc['arbiter']];
                that.header += "        {." + di + "=" + that.parse_value(trn['value'][j], true) + "},\n";
            } );
            that.header += "    };\n";
            that.header += "    spop2_add(*(population *)pop, q, num);\n";
            that.header += "}\n";
            that.header += "\n";
        } );
    }
    write_custom() {
        let that = this;
        this.json['populations'].forEach( (pop) => {
            if (!('processes' in pop) || (pop['processes'].length == 0)) return;
            pop['processes'].forEach( (proc) => {
                if ('hazard' in proc) {
                    that.header += "double fun_hazard_" + proc['id'] + "_" + pop['id'] + "(hazard hfun, unsigned int d, number q, number k, double theta, const number *key) {\n";
                    that.header += "    double devmn = " + that.parse_value(proc['hazard'][1], true) + ";\n";
                    that.header += "    double devsd = " + that.parse_value(proc['hazard'][2], true) + ";\n";
                    that.header += "    hazpar hz = " + hazard[proc['hazard'][0]][0] + "(devmn, devsd);\n";
                    that.header += "    double a = " + hazard[proc['hazard'][0]][1] + "(" + hazard[proc['hazard'][0]][2] + ", 0, key[" + proc['id'] + "], hz.k, hz.theta, key);\n";
                    that.header += "    return a;\n";
                    that.header += "}\n";
                    that.header += "\n";
                }
                if (('hazpar' in proc) && proc['hazpar']) {
                    that.header += "void fun_hazpar_" + proc['id'] + "_" + pop['id'] + "(const number *key, const number num, double *par) {\n";
                    proc['value'].forEach( (val, i) => {
                        that.header += "    par[" + util.format(i) + "] = " + that.parse_value(val, true) + ";\n";
                    } );
                    that.header += "}\n";
                    that.header += "\n";
                }
            } );
        } );        
    }
    write_init() {
        this.model += "void init(int *no, int *np, int *ni) {\n";
        if (this.json['model']['type'] == "Population") {
            if (!this.deterministic) {
                this.model += "    spop2_random_init();\n";
                this.model += "\n";
            }
            if ('istep' in this.json['model']['parameters']) {
                this.model += "    spop2_set_eps(" + util.format(this.json['model']['parameters']['istep']) + ");\n";
                this.model += "\n";
            }
        }
        this.model += "    *no = NumPop;\n";
        this.model += "    *np = NumPar;\n";
        this.model += "    *ni = NumInt;\n";
        this.model += "}\n";
        this.model += "\n";
    }
    write_parnames() {
        let that = this;
        this.model += "void parnames(char **names, double *param, double *parmin, double *parmax) {\n";
        this.model += "    char temp[NumPop+NumPar+NumInt][256] = {\n";
        if (this.numpop > 0) {
            this.model += "        \"" + this.json['populations'].map( (s) => s['id'] ).join("\", \"") + "\",\n";
        }
        if (this.numpar > 0) {
            this.model += "        \"" + this.json['parameters'].filter( (p) => !p['constant'] ).map( (pr) => pr['id'] ).join("\", \"") + "\",\n";
        }
        if (this.numint_int > 0) {
            this.model += "        \"" + this.json['intermediates'].map( (pr) => pr['id'] ).join("\", \"") + "\",\n";
        }
        if (this.numint_trans > 0) {
            this.model += "        \"" + this.json['transformations'].map( (pr) => pr['id'] ).join("\", \"") + "\",\n";
        }
        this.model += "    };\n";
        this.model += "\n";
        this.model += "    int i;\n";
        this.model += "    for (i=0; i<(NumPop+NumPar+NumInt); i++)\n";
        this.model += "        names[i] = strdup(temp[i]);\n";
        this.model += "\n";
        this.json['parameters'].filter( (p) => !p['constant'] ).forEach( (pr, i) => {
            that.model += "    param[" + pr['id'] + "] = " + util.format('value' in pr ? this.parse_value(pr['value']) : 0.0) + ";\n";
            that.model += "    parmin[" + pr['id'] + "] = " + util.format('min' in pr ? this.parse_value(pr['min']) : ('value' in pr ? this.parse_value(pr['value']) : 0.0)) + ";\n";
            that.model += "    parmax[" + pr['id'] + "] = " + util.format('max' in pr ? this.parse_value(pr['max']) : ('value' in pr ? this.parse_value(pr['value']) : 0.0)) + ";\n";
        } );
        this.model += "}\n";
        this.model += "\n";
    }
    write_destroy() {
        this.model += "void destroy(void) {\n";
        if (this.json['model']['type'] == "Population" && !this.deterministic) {
                this.model += "    spop2_random_destroy();\n";
        }
        this.model += "}\n";
        this.model += "\n";
    }
    write_out(tab, iret=true) {
        let that = this;
        if (this.deterministic) {
            this.json['populations'].forEach( (spc, i) => {
                that.model += "    ".repeat(tab) + "ret[" + util.format(i) + "] = size_" + spc['id'] + ".d;\n" + "    ".repeat(tab) + "if (CHECK(ret[" + util.format(i) + "])) {goto endall;};\n";
            } );
        } else {
            this.json['populations'].forEach( (spc, i) => {
                that.model += "    ".repeat(tab) + "ret[" + util.format(i) + "] = (double)(size_" + spc['id'] + ".i);\n" + "    ".repeat(tab) + "if (CHECK(ret[" + util.format(i) + "])) {goto endall;};\n";
            } );
        }
        this.model += "\n";
        this.model += "    ".repeat(tab) + "ret += " + (util.format(this.json['populations'].length)) + ";\n";
        this.model += "\n";
        if (iret) {
            if ('intermediates' in this.json) {
                this.json['intermediates'].forEach( (spc, i) => {
                    that.model += "    ".repeat(tab) + "iret[" + util.format(i) + "] = " + spc['id'] + ";\n" + "    ".repeat(tab) + "if (CHECK(iret[" + util.format(i) + "])) {goto endall;};\n";
                } );
                this.model += "\n";
            }
            if ('transformations' in this.json) {
                this.json['transformations'].forEach( (spc, i) => {
                    that.model += "    ".repeat(tab) + "iret[" + util.format(this.json['intermediates'].length + i) + "] = " + spc['id'] + ";\n" + "    ".repeat(tab) + "if (CHECK(iret[" + util.format(i) + "])) {goto endall;};\n";
                } );
                this.model += "\n";
            }
            if ('intermediates' in this.json) {
                this.model += "    ".repeat(tab) + "iret += " + (util.format(this.json['intermediates'].length + this.json['transformations'].length)) + ";\n";
            }
            this.model += "\n";
        }
    }
    write_sim() {
        let i, j;
        let that = this;
        let det = this.deterministic ? 'DETERMINISTIC' : 'STOCHASTIC';
        this.model += "void sim(int tf, int rep, double *envir, double *pr, double *y0, const char *file0, const char *file1, double *ret, double *iret, int *success) {\n";
        this.model += "\n";
        this.model += "    int TIME = 0;\n";
        this.model += "\n";
        this.model += "    model_param = pr;\n";
        if ('environ' in this.json) {
           this.json['environ'].forEach( (elm, i) => {
                that.model += "    envir_" + elm['id'] + " = envir + " + util.format(i) + " * tf;\n";
           });
           this.model += "\n";
        }
        //
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (spc) => {
                that.model += "    population " + util.format(spc['id']) + ";\n";
            });
            this.model += "\n";
        }
        if (this.json['model']['type'] == "Population") {
            this.model += "    number num = numZERO;\n";
            this.model += "    char arbiters[" + util.format(this.numproc) + "];\n";
            this.model += "    number key[" + util.format(this.numproc) + "];\n";
            this.json['populations'].forEach( (spc, i) => {
                that.model += "    number size_" + spc['id'] + ";\n";
            } );
            this.json['populations'].forEach( (spc, i) => {
                if (spc['processes'].length == 0) return;
                //
                that.model += "    number completed_" + spc['id'] + "[" + util.format(that.numproc) + "];\n";
            } );
            this.json['populations'].forEach( (spc, i) => {
                if (that.transfers && that.transfers.includes(spc['id'])) {
                    that.model += "    population popdone_" + spc['id'] + "[" + util.format(that.numproc) + "];\n";
                }
            } );
            if ('transformations' in this.json) {
                if (this.deterministic) {
                    this.json['transformations'].map( (trx) => trx['id'] ).forEach( (id) => {
                        that.model += "    " + id + " = 0.0;\n";
                    } );
                } else {
                    this.json['transformations'].map( (trx) => trx['id'] ).forEach( (id) => {
                        that.model += "    " + id + " = 0;\n";
                    } );
                }
            }
            this.model += "    double par[" + util.format(this.numprocpar) + "];\n";
            this.model += "\n";
            //
            this.model += "    FILE *file;\n";
            this.model += "    number *buff = 0;\n";
            this.model += "    unsigned int buffsz = 0;\n";
            this.model += "    if (file0 && file0[0]!=' ') {\n";
            this.model += "        file = fopen(file0,\"rb\");\n";
            this.model += "        if (!file) {\n";
            this.model += "            *success = 0;\n";
            this.model += "            goto endall;\n";
            this.model += "        }\n";
            this.model += "        rewind(file);\n";
            this.model += "    }\n";
            this.model += "\n";
            this.model += "    if (file0 && file0[0]!=' ') {\n";
            this.json['populations'].forEach( (spc, i) => {
                that.model += "        fread(&buffsz, sizeof(unsigned int), 1, file);\n";
                that.model += "        buff = (number *)malloc(buffsz);\n";
                that.model += "        fread(buff, buffsz, 1, file);\n";
                that.model += "        " + spc['id'] + " = spop2_loadstate(buff);\n";
                that.model += "\n";
                if (that.transfers && that.transfers.includes(spc['id'])) {
                    for (j=0; j<that.numproc; j++) {
                        that.model += "        popdone_" + spc['id'] + "[" + util.format(j) + "] = spop2_loadstate_empty(buff);\n";
                    }
                }
                that.model += "\n";
                that.model += "        free(buff);\n";
                that.model += "\n";
            } );
            this.model += "    } else {\n";
            this.json['populations'].forEach( (spc, i) => {
                for (j=0; j<that.numproc; j++) {
                    that.model += "        arbiters[" + util.format(j) + "] = " + (j < spc['processes'].length ? spc['processes'][j]['arbiter'] : "STOP") + ";\n";
                    that.model += "        key[" + util.format(j) + "] = numZERO;\n";
                }
                that.model += "        " + spc['id'] + " = spop2_init(arbiters, " + det + ");\n";
                for (j=0; j<that.numproc; j++) {
                    if ((j < spc['processes'].length) && ('stepper' in spc['processes'][j])) {
                        if (!(spc['processes'][j]['stepper'] in stepper)) {
                            that.error += "Stepper not defined. Please choose one of these: " + Object.keys(stepper).join(", ") + "\n";
                            break;
                        }
                        that.model += "        " + spc['id'] + "->arbiters[" + util.format(j) + "]->fun_step = " + stepper[spc['processes'][j]['stepper']] + ";\n";
                    }
                    if (j < spc['processes'].length) {
                        if ('hazard' in spc['processes'][j]) {
                            if (!(spc['processes'][j]['hazard'][0] in hazard)) {
                                that.error += "Hazard not defined. Please choose one of these: " + Object.keys(hazard).join(", ") + "\n";
                                break;
                            }
                            that.model += "        " + spc['id'] + "->arbiters[" + util.format(j) + "]->fun_calc = fun_hazard_" + spc['processes'][j]['id'] + "_" + spc['id'] + ";\n";
                        }
                        if ('hazpar' in spc['processes'][j]) {
                            that.model += "        " + spc['id'] + "->arbiters[" + util.format(j) + "]->fun_q_par = fun_hazpar_" + spc['processes'][j]['id'] + "_" + spc['id'] + ";\n";
                        }
                    }
                }
                that.model += "        if (y0[" + util.format(i) + "]) { num." + ("i",that.deterministic ? "d" : "i") + " = y0[" + util.format(i) + "]; spop2_add(" + spc['id'] + ", key, num); }\n";
                that.model += "\n";
                if (that.transfers && that.transfers.includes(spc['id'])) {
                    for (j=0; j<that.numproc; j++) {
                        that.model += "        popdone_" + spc['id'] + "[" + util.format(j) + "] = spop2_init(arbiters, " + det + ");\n";
                    }
                }
                this.model += "\n";
            });
            that.model += "    }\n";
            this.model += "\n";
            this.model += "    if (file0 && file0[0]!=' ') {\n";
            this.model += "        fclose(file);\n";
            this.model += "    }\n";
        }
        //
        if ('intermediates' in this.json) {
            this.json['intermediates'].forEach( (elm, i) => {
                that.model += "    " + elm['id'] + " = 0.0;\n";
            });
            this.model += "\n";
        }
        //
        this.json['populations'].forEach( (spc, i) => {
            that.model += "    size_" + spc['id'] + " = spop2_size(" + spc['id'] + ");\n";
        } );
        this.model += "\n";
        this.write_out(1, false);
        this.model += "    for (TIME=1; TIME<tf; TIME++) {\n";
        //
        if ('intermediates' in this.json) {
            this.json['intermediates'].forEach( (elm) => {
                that.model += "        " + elm['id'] + " = " + that.parse_value(elm['value']) + ";\n";
            });
            this.model += "\n";
        }
        //
        this.model += "        if (rep >= 0) {\n";
        //
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (spc, i) => {
                if (spc['processes'].length == 0) return;
                //
                let pars = [];
                spc['processes'].forEach( (s) => {
                    if (Array.isArray(s['value'])) {
                        s['value'].forEach( (ss) => pars.push((('hazpar' in s) && s['hazpar']) ? "0.0" : ss) );
                    } else {
                        pars.push((('hazpar' in s) && s['hazpar']) ? "0.0" : s['value']);
                    }
                } );
                for (j = 0; j < that.numprocpar; j++) {
                    that.model += "                par[" + util.format(j) + "] = " + (pars.length ? that.parse_value(pars.shift()) : "0.0") + ";\n";
                }
                that.model += "                spop2_step(" + spc['id'] + ", par, &size_" + spc['id'] + ", completed_" + spc['id'] + ", " + (that.transfers.includes(spc['id']) ? "popdone_" + spc['id'] : "0") + ");\n";
                that.model += "\n";
            } );
            //
            /*
                TO DO!
                THERE SHOULD BE AN ORDER TO THESE PROCESSES:
                    INTERMEDIATES
                    TRANSFORMATIONS
                    TRANSFERS
            */
            if ('transformations' in this.json) {
                this.json['transformations'].forEach( (trx) => {
                    that.model += "                " + trx['id'] + " = " + that.parse_value(trx['value']) + ";\n";
                } );
                that.model += "\n";
                //
                this.json['transformations'].filter( (trx) => 'to' in trx ).forEach( (trx) => {
                    for (j=0; j<that.numproc; j++) {
                        that.model += "                key[" + util.format(j) + "] = numZERO;\n";
                    }
                    that.model += "                num" + (that.deterministic ? ".d" : ".i") + " = " + trx['id'] + ";\n";
                    that.model += "                spop2_add(" + trx['to'] + ", key, num);\n";
                    that.model += "\n";
                } );
                //
                this.json['transformations'].filter( (trx) => 'to' in trx ).forEach( (trx) => {
                    if (that.deterministic) {
                        that.model += "                size_" + trx['to'] + ".d += " + trx['id'] + ";\n";
                    } else {
                        that.model += "                size_" + trx['to'] + ".i += " + trx['id'] + ";\n";
                    }
                } );
                that.model += "\n";
            }
            //
            if ('transfers' in this.json) {
                this.json['transfers'].forEach( (trn) => {
                    that.model += "                spop2_foreach(popdone_" + that.processobj[trn['from']]['parent_id'] + "[" + trn['from'] + "], fun_transfer_" + trn['id'] + ", (void *)(&" + trn['to'] + "));\n";
                } );
                that.model += "\n";
                this.transfers.forEach( (trn) => {
                    for (j=0; j<that.numproc; j++) {
                        that.model += "                spop2_empty(&popdone_" + trn + "[" + util.format(j) + "]);\n"
                    }
                    that.model += "\n";
                } );
                Array.from(new Set(this.json['transfers'].map( (trn) => trn['to'] ))).forEach( (trn) => {
                    that.model += "                size_" + trn + " = spop2_size(" + trn + ");\n";
                } );
                that.model += "\n";
            }
            //
            that.model += "        }\n";
            that.model += "\n";
            //
            this.write_out(2, true);
        }
        //
        this.model += "    }\n";
        this.model += "\n";
        this.model += "  endall:\n";
        this.model += "\n";
        this.model += "    *success = TIME;\n";
        this.model += "\n";
        this.model += "    if (file1 && file1[0]!=' ') {\n";
        this.model += "        file = fopen(file1,\"wb\");\n";
        this.model += "        if (!file) {\n";
        this.model += "            *success = 0;\n";
        this.model += "        } else {\n";
        this.model += "            rewind(file);\n";
        this.model += "\n";
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (spc) => {
                that.model += "            buffsz = spop2_buffsize(" + spc['id'] + ");\n";
                that.model += "            buff = spop2_savestate(" + spc['id'] + ");\n";
                that.model += "            fwrite(&buffsz, sizeof(unsigned int), 1, file);\n";
                that.model += "            fwrite(buff, buffsz, 1, file);\n";
                that.model += "            free(buff);\n";
                that.model += "\n";
            });
        }
        this.model += "            fclose(file);\n";
        this.model += "        }\n";
        this.model += "    }\n";
        this.model += "\n";
        if (this.json['model']['type'] == "Population") {
            this.json['populations'].forEach( (spc) => {
                that.model += "    spop2_free(&" + spc['id'] + ");\n";
                if (that.transfers && that.transfers.includes(spc['id'])) {
                    for (j=0; j<that.numproc; j++) {
                        that.model += "    spop2_free(&(popdone_" + spc['id'] + "[" + util.format(j) + "]));\n";
                    }
                }
            });
            this.model += "\n";
        }
        this.model += "}\n";
        this.model += "\n";
    }
    write_main() {
        this.model += "int main(int argc, char *argv[]) {\n";
        this.model += "    return 0;\n";
        this.model += "}\n";
        this.model += "\n";
    }
    write_funcount(funname, context) {
        this.header += "char " + funname + "(number *key) {\n";
        this.header += "    return " + context + ";\n";
        this.header += "}\n";
        this.header += "\n";
    }
    parse_value(value, transfers=false) {
        let that = this;
        if (Array.isArray(value)) { // Function
            let fun = this.parse_value(value[0], transfers=transfers);
            if (fun == "define") {
                if (!(value[1].every( (v) => that.funparnames.includes(v) ))) {
                    this.error += "Error in function definition\nYou are allowed to use these as parameter names:\n" + that.funparnames + "\nERROR: " + fun + " : " + value + "\n";
                    return "";
                }
                let def = this.parse_value(value[2], transfers=transfers);
                return "(" + value[1].join(",") + ") (" + def + ")";
            } else if (fun == "count") {
                let pop = this.parse_value(value[1], transfers=transfers);
                if (!that.json['populations'].filter( (tmp) => tmp['id'] == value[1] )[0]) {
                    this.error += "Error in count request\nHere is the correct usage:\n[\"count\", \"population name\", [condition]]\n";
                    return "";
                }
                this.funcountid += 1;
                let funname = "fun_count_" + pop + "_" + this.funcountid;
                let context = this.parse_value(value[2], transfers=true);
                this.write_funcount(funname,context)
                return "spop2_count(" + pop + ", " + funname + ")" + (this.deterministic ? ".d" : ".i");
            } else {
                let prm = value.slice(1).map( (v) => that.parse_value(v, transfers=transfers) );
                if (this.processes.includes(fun)) {
                    if (transfers) {
                        return "key[" + fun + "]." + this.popart[prm[0]][this.processobj[fun]['arbiter']];
                    }
                    return "completed_" + prm[0] + "[" + fun + "]" + (this.deterministic ? ".d" : ".i");
                } else if (fun == "size") {
                    return "size_" + prm[0] + (this.deterministic ? ".d" : ".i");
                } else if (fun == "index") {
                    return prm[0] + "[" + prm[1] + "]";
                } else if (fun == "round") {
                    return fun + "(" + prm.join(", ") + ")";
                } else if (fun == "min") {
                    return "dmin(" + prm.join(", ") + ")";
                } else if (fun == "max") {
                    return "dmax(" + prm.join(", ") + ")";
                } else if (this.functions.includes(fun) || fun == "exp" || fun == "log" || fun == "log2" || fun == "log10" || fun == "pow" || fun == "sqrt") {
                    return fun + "(" + prm.join(", ") + ")";
                } else if (fun == "binomial") {
                    return "(unsigned int)gsl_ran_binomial(RANDOM, " + prm[0] + ", " + prm[1] + ")";
                } else if (fun == "poisson") {
                    return "(unsigned int)gsl_ran_poisson(RANDOM, " + prm[0] + ")";
                } else if (fun == "*") {
                    return "(" + prm.join(" * ") + ")";
                } else if (fun == "+") {
                    return "(" + prm.join(" + ") + ")";
                } else if (fun == "-") {
                    return "(" + prm.join(" - ") + ")";
                } else if (fun == "/") {
                    return "(" + prm.join(" / ") + ")";
                } else if (fun == "?") {
                    return "((" + prm[0] + ") ? (" + prm[1] + ") : (" + prm[2] + "))";
                } else if (fun == "&&") {
                    return "(" + prm.join(" && ") + ")";
                } else if (fun == "||") {
                    return "(" + prm.join(" || ") + ")";
                } else if (fun == "indicator") {
                    return this.deterministic ? "(" + prm[0] + " ? 1.0 : 0.0)" : "(" + prm[0] + " ? 1 : 0)";
                } else if (fun == "==") {
                    return "(" + prm[0] + " == " + prm[1] + ")";
                } else if (fun == ">") {
                    return "(" + prm[0] + " > " + prm[1] + ")";
                } else if (fun == "<") {
                    return "(" + prm[0] + " < " + prm[1] + ")";
                } else if (fun == ">=") {
                    return "(" + prm[0] + " >= " + prm[1] + ")";
                } else if (fun == "<=") {
                    return "(" + prm[0] + " <= " + prm[1] + ")";
                } else {
                    this.error += "Unknown keyword in equation\nERROR: " + fun + " : " + value + "\n";
                    return "";
                }
            }
        } else { // Parameter
            if (typeof value === 'string' || value instanceof String) { // String
                if (this.environs.includes(value)) {
                    return "envir_"+value;
                } else if (this.populations.includes(value)) {
                    return value;
                } else if (this.processes.includes(value)) {
                    return value;
                } else if (this.parametersv.includes(value)) {
                    return "model_param[" + value + "]";
                } else if (this.parametersc.includes(value)) {
                    return value;
                } else if (this.functions.includes(value)) {
                    return value;
                } else if (this.intermediates.includes(value)) {
                    return value;
                } else if (this.transformations.includes(value)) {
                    return value;
                } else if (this.operations.includes(value)) {
                    return value;
                } else if (this.funparnames.includes(value)) {
                    return "(" + value + ")";
                } else if (value == "TIME") {
                    return "TIME";
                } else if (value == "TIME_1") {
                    return "(TIME-1)";
                } else if (!isNaN(parseFloat(value))) {
                    return value;
                } else if (!isNaN(parseInt(value))) {
                    return value;
                } else {
                    this.error += "Unknown reference to a user defined keyword\nERROR: " + value + "\n";
                    return "";
                }
            } else if (typeof value === 'number') { // Number
                return value.toString();
            } else { // NaN
                this.error += "ERROR: Invalid value encountered" + fun + " : " + value + "\n";
                return "";
        }
        }
    }
}

exports.PopJSON = PopJSON;

},{"fs":1,"util":5}]},{},["PopJSON"]);
