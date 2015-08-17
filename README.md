# libclockhack
Intercept (LD_PRELOAD) library to set the time back/forward for a process 
libclockhack — set the time backward/forward for a process by intercepting the relevant system calls.

A small shared library that intercepts the three system calls that linux processes use to get the current time
viz

time_t time (time_t* t);                                       -- time(2)

int    gettimeofday (struct timeval* tv, struct timezone* tz); -- gettimeofday(2)

int    clock_gettime (clockid_t clk_id, struct timespec* ts);  -- clock_gettime(2)


The offset in seconds from the current time is passed via the environment variable HACK_CLOCK_SHIFT.
Note: the apparent time still increases monotonically at a constant offset from the real time.

Examples

 # date uses clock_gettime(2)
/bin/date
Mon Apr  8 21:02:35 EST 2013

LD_PRELOAD=`pwd`/libclockhack.so HACK_CLOCK_SHIFT=-100000000 /bin/date
Sat Feb  6 11:16:25 EST 2010


BUGS

Should be able to fiddle it to work on most unix systems.
If RTLD_NEXT isn't supported then accessing the real system calls using the syscall(2)
interface is another option.

NetBSD seems to rename gettimeofday to gettimeofday50 and has a slightly different prototype
but does work after making this change.

LICENSE
Creative Commons CC0 http://creativecommons.org/publicdomain/zero/1.0/legalcode

AUTHOR
James Sainsbury

SEE ALSO
