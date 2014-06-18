nsenter
=======

use node to set nice values of processes.

API
---
```
getPriority(which, who)
setPriority(which, who, prio)
```
DESCRIPTION
-----------
       The  scheduling  priority  of  the  process, process group, or user, as
       indicated by which and who is obtained with the getpriority() call  and
       set with the setpriority() call.

       `which` is a string with a value PRIO_PROCESS, PRIO_PGRP, or PRIO_USER,
       and who is interpreted relative to  which  (a  process  identifier  for
       PRIO_PROCESS, process group identifier for PRIO_PGRP, and a user ID for
       PRIO_USER).  A zero value for who denotes  (respectively)  the  calling
       process,  the process group of the calling process, or the real user ID
       of the calling process.  Prio is a value in the range -20  to  19  (but
       see  the  Notes  below).   The  default priority is 0; lower priorities
       cause more favorable scheduling.

       The getpriority() call returns the highest priority  (lowest  numerical
       value)  enjoyed  by  any of the specified processes.  The setpriority()
       call sets the priorities of all  of  the  specified  processes  to  the
       specified value.  Only the superuser may lower priorities.

RETURN VALUE
------------
       Since  getpriority()  can  legitimately  return  the  value  -1,  it is
       necessary to clear the external variable errno prior to the call,  then
       check  it  afterward  to  determine  if  -1 is an error or a legitimate
       value.  The setpriority() call returns 0 if there is no error, or -1 if
       there is.