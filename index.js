var util = require('./build/Release/util');

// which is encoded like so:
// PRIO_PROCESS = 1
// PRIO_PGRP = 2
// PRIO_USER = 3
function whichEncode(which) {
  switch (which) {
    case 'PRIO_PROCESS':
      return 1;
    case 'PRIO_PGRP':
      return 2;
    case 'PRIO_USER':
      return 3;
  }
}

function getPriority(which, who) {
  return util.getPriority(whichEncode(which), who);
}

function setPriority(which, who, prio) {
  return util.setPriority(whichEncode(which), who, prio);
}

module.exports.getPriority = getPriority;
module.exports.setPriority = setPriority;
