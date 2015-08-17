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

enum	{
	false	= 0,
	true	= 1,
	err	= -1,
	ok	= 0,
};

static	struct	{
	int	done;
	time_t	(*time)(time_t*);
	int	(*gettimeofday)(struct timeval* tv, struct timezone* tz);
	int	(*clock_gettime)(clockid_t clk_id, struct timespec *tp);
}	original	= {false, 0, 0, 0};

static	void	hack_clock_init() {
	char*	env	= getenv (HACK_CLOCK_SHIFT);
	if (env) {
		clock_shift		= strtol (env, 0, 0);
	}
	original.time		= dlsym (RTLD_NEXT, "time");
	original.gettimeofday	= dlsym (RTLD_NEXT, "gettimeofday");
	original.clock_gettime	= dlsym (RTLD_NEXT, "clock_gettime");
	original.done		= true;
}

/* Forward declarations */
static	time_t	time_init (time_t* t);
static	int	gettimeofday_init (struct timeval *tv, struct timezone *tz);
static	int	clock_gettime_init (clockid_t clk_id, struct timespec *tp);

/* Addresses of intercept functions - updated after first use */
static	struct {
	time_t	(*time)(time_t*);
	int	(*gettimeofday) (struct timeval *tv, struct timezone *tz);
	int	(*clock_gettime) (clockid_t clk_id, struct timespec *tp);
}
intercept	= {
	.time 		= time_init,
	.gettimeofday	= gettimeofday_init,
	.clock_gettime	= clock_gettime_init
};

time_t	time (time_t* t)	{
	return	intercept.time (t);
}
# if	!defined(GETTIMEOFDAY50) 
int	gettimeofday(struct timeval *tv, struct timezone *tz) 
# else
int	gettimeofday50(struct timeval* tv, void * tz) 
# endif
{
	return	intercept.gettimeofday (tv, tz);
}
int clock_gettime (clockid_t clk_id, struct timespec *tp) {
	return	intercept.clock_gettime (clk_id, tp);
}

static	time_t	shifted_time (time_t* t) {
	time_t	result	= (time_t)(err);
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
static	time_t	time_init (time_t* t){
	if (!original.done)
		hack_clock_init ();
	intercept.time	= shifted_time;
	return	intercept.time(t);
}
	
static	int	shifted_gettimeofday (struct timeval *tv, struct timezone *tz) {
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
static	int	gettimeofday_init (struct timeval *tv, struct timezone *tz) {
	if (!original.done)
		hack_clock_init ();
	intercept.gettimeofday	= shifted_gettimeofday;
	return	intercept.gettimeofday (tv, tz);
}

static	int shifted_clock_gettime (clockid_t clk_id, struct timespec *tp) {
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
static	int clock_gettime_init (clockid_t clk_id, struct timespec *tp) {
	if (!original.done)
		hack_clock_init ();
	intercept.clock_gettime	= shifted_clock_gettime;
	return	intercept.clock_gettime (clk_id, tp);
}
