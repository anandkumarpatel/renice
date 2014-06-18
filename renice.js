#!/usr/bin/env node
var program = require('commander');
var renice = require('./index.js');

program
  .version('0.0.1')
  .usage('renice priority [[-p] pid ...] [[-g] pgrp ...] [[-u] user ...] \n\t renice -n increment [[-p] pid ...] [[-g] pgrp ...] [[-u] user ...]')
  .option('-p, --pid [n]', "Reset the who interpretation to be (the default) process IDs", parseInt)
  .option('-g, --pgrp [n]', "Force who parameters to be interpreted as process group ID's", parseInt)
  .option('-u, --user [n]', "Force the who parameters to be interpreted as user names or user ID's", parseInt)
  .option('-n, --increment [n]', "Instead of changing the specified processes to the given priority, interpret the following argument as an increment to be applied to the current priority of each process.", parseInt)
  .option('-m, --negative value [n]', "hack to allow negative values", parseInt)
  .parse(process.argv);

var doIncrement = false;
var which = 'PRIO_PROCESS';
var who = null;
var prio = 0;

if (typeof program.increment === 'number') {
  doIncrement = true;
} else if (typeof program.negative === 'number' ) {
  prio = -program.negative;
} else if (typeof program.args[0] === 'string') {
  prio = parseInt(program.args[0]);
  if (typeof prio !== 'number') {
    console.error("need to provide nice value to set");
    program.usage();
    process.exit(1);
  }
}

if (typeof program.pgrp === 'number') {
  which = 'PRIO_PGRP';
  who = program.pgrp;
} else if (typeof program.user === 'number') {
  which = 'PRIO_USER';
  who = program.user;
} else if (typeof program.pid === 'number') {
  which = 'PRIO_PROCESS';
  who = program.pid;
}

if (!who) {
  console.error("no process defined");
  process.exit(1);
}

try {
  var oldPrio = renice.getPriority(which, who);
} catch (err) {
  console.error(err);
  process.exit(1);
}

if (doIncrement) {
  prio = oldPrio + program.increment;
}

var err = 0;
try {
  err = renice.setPriority(which, who, prio);
} catch (err) {
  console.error(err);
  process.exit(1);
}

if (err) {
  console.error("errno", err);
  process.exit(1);
}

console.log("old priority:", oldPrio, "new priority", prio);
