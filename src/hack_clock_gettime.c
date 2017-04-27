/*
//	@(#) hack_clock_gettime.c
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

# include	"constant.h"

extern	long	__attribute__((visibility("hidden"))) clock_shift;
static	int	(*original_clock_gettime)(clockid_t clk_id, struct timespec *tp);
static	void	__attribute__((constructor))	hack_clock_gettime_init() {
	original_clock_gettime	= dlsym (RTLD_NEXT, "clock_gettime");
}

static	int intercept_clock_gettime (clockid_t clk_id, struct timespec *tp) {
	int	result	= err;
	if (original_clock_gettime) {
		struct	timespec	t	= {0, 0};
		result	= original_clock_gettime (clk_id, &t);
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
int	__attribute__((alias("intercept_clock_gettime"))) clock_gettime (clockid_t clk_id, struct timespec *tp);
