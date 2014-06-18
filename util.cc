#include <node.h>
#include <sched.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace v8;

// which is decoded like so:
// PRIO_PROCESS = 1
// PRIO_PGRP = 2
// PRIO_USER = 3
static int whichDecode(int which)
{
  switch (which)
  {
    case 1:
      return PRIO_PROCESS;
      break;
    case 2:
      return PRIO_PGRP;
      break;
    case 3:
      return PRIO_USER;
      break;
    default:
      return PRIO_PROCESS;
      break;
  }
  return PRIO_PROCESS;
}

static int prioCheck(int prio)
{
  if (prio > PRIO_MAX)
    prio = PRIO_MAX;
  if (prio < PRIO_MIN)
    prio = PRIO_MIN;
  return prio;
}

Handle<Value> setPriority(const Arguments& args) {
  HandleScope scope;

  if (args.Length() != 3) {
    ThrowException(Exception::TypeError(String::New("should only have one argument")));
    return scope.Close(Undefined());
  }

  int which = args[0]->Int32Value();
  if (!args[0]->IsNumber() || which < 1 || which > 3) {
    ThrowException(Exception::TypeError(String::New("argument which should be a number value between 1 and 3")));
    return scope.Close(Undefined());
  }

  int who = args[1]->Int32Value();
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(String::New("argument who should be a number")));
    return scope.Close(Undefined());
  }

  int prio = args[2]->Int32Value();
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(String::New("argument prio should be a number")));
    return scope.Close(Undefined());
  }

  which = whichDecode(which);
  prio = prioCheck(prio);
  errno = 0;
  if (setpriority(which, who, prio) < 0) {
    return scope.Close(Number::New(errno));
  }

  return scope.Close(Number::New(0));
}

Handle<Value> getPriority(const Arguments& args) {
  HandleScope scope;

  if (args.Length() != 2) {
    ThrowException(Exception::TypeError(String::New("should only have one argument")));
    return scope.Close(Undefined());
  }

  int which = args[0]->Int32Value();
  if (!args[0]->IsNumber() || which < 1 || which > 3) {
    ThrowException(Exception::TypeError(String::New("argument which should be a number value between 1 and 3")));
    return scope.Close(Undefined());
  }

  int who = args[1]->Int32Value();
  if (!args[0]->IsNumber()) {
    ThrowException(Exception::TypeError(String::New("argument who should be a number")));
    return scope.Close(Undefined());
  }


  which = whichDecode(which);
  errno = 0;
  int currentPri = getpriority(which, who);
  if (currentPri == -1 && errno) {
    ThrowException(Exception::Error(String::New("errno")));
    return scope.Close(Number::New(errno));
  }

  return scope.Close(Number::New(currentPri));
}

void Init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("setPriority"), FunctionTemplate::New(setPriority)->GetFunction());
  exports->Set(String::NewSymbol("getPriority"), FunctionTemplate::New(getPriority)->GetFunction());
}

NODE_MODULE(util, Init)
