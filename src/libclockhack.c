/*
//	@(#) libclockhack.c
//
//	License: Creative Commons CC0 
//		http://creativecommons.org/publicdomain/zero/1.0/legalcode
//	Author:	James Sainsbury
//		toves@sdf.lonestar.org
//
*/
# define	_GNU_SOURCE
# include	<dlfcn.h>

# include	<sys/time.h>
# include	<time.h>
# include	<errno.h>
# include	<stdlib.h>

/* Name of the environment variable */
static	char	HACK_CLOCK_SHIFT[]	= "HACK_CLOCK_SHIFT";

/* plus/minus offset from 'now' in seconds */
static	long	clock_shift	= 0;

enum	{ err	= -1, ok	= 0, };

static	struct	{
	time_t	(*time)(time_t*);
	int	(*gettimeofday)(struct timeval* tv, struct timezone* tz);
	int	(*clock_gettime)(clockid_t clk_id, struct timespec *tp);
}	original	= { 0, 0, 0 };

static	void	__attribute__((constructor))	hack_clock_init() {

	char*	env	= getenv (HACK_CLOCK_SHIFT);
	if (env) {
		clock_shift		= strtol (env, 0, 0);
	}
	original.time		= dlsym (RTLD_NEXT, "time");
	original.gettimeofday	= dlsym (RTLD_NEXT, "gettimeofday");
	original.clock_gettime	= dlsym (RTLD_NEXT, "clock_gettime");
}

/* Forward declarations */
static	time_t	intercept_time(time_t* t);
static	int	intercept_gettimeofday(struct timeval *tv, struct timezone *tz);
static	int	intercept_clock_gettime(clockid_t clk_id, struct timespec *tp);


time_t	time (time_t* t)	{
	return	intercept_time (t);
}

int	gettimeofday(struct timeval *tv, struct timezone *tz) 
{
	return	intercept_gettimeofday (tv, tz);
}

int clock_gettime (clockid_t clk_id, struct timespec *tp) {
	return	intercept_clock_gettime (clk_id, tp);
}

static	time_t	intercept_time (time_t* t) {
	time_t	result	= (time_t)(-1);
	if (original.time) {
		result	= original.time (0);
		if (result != (time_t)(-1)) {
			result	+= clock_shift;
			if (t) {
				*t	= result;
			}
		}
	}
	else	errno	= ENOSYS;

	return	result;
}
	
static	int	intercept_gettimeofday (struct timeval *tv, struct timezone *tz) {
	int	result	= err;
	if (original.gettimeofday) {
		struct	timeval	t	= {0, 0};
		result	= original.gettimeofday (&t, tz);
		if (result == ok) {
			t.tv_sec	+= clock_shift;
			if (tv) {
				*tv	= t;
			}
			else	{
				errno	= EFAULT;
				result	= err;
			}
		}
	}
	else	errno	= ENOSYS;

	return	result;
}

static	int intercept_clock_gettime (clockid_t clk_id, struct timespec *tp) {
	int	result	= err;
	if (original.clock_gettime) {
		struct	timespec	t	= {0, 0};
		result	= original.clock_gettime (clk_id, &t);
		if (result == ok) {
			if (clk_id == CLOCK_REALTIME) {
				t.tv_sec	+= clock_shift;
			}
			if (tp) {
				*tp	= t;
			}
			else	{
				errno	= EFAULT;
				result	= err;
			}
		}
	}
	else	errno	= ENOSYS;

	return	result;
}
