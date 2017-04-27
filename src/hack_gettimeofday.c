/*
//	@(#) hack_gettimeofday.c
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


static	int	(*original_gettimeofday)(struct timeval* tv, struct timezone* tz);

static	void	__attribute__((constructor))	hack_gettimeofday_init() {
	original_gettimeofday	= dlsym (RTLD_NEXT, "gettimeofday");
}
static	int	intercept_gettimeofday (struct timeval *tv, struct timezone *tz) {
	int	result	= err;
	if (original_gettimeofday) {
		struct	timeval	t	= {0, 0};
		result	= original_gettimeofday (&t, tz);
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

int	__attribute__((alias("intercept_gettimeofday"))) gettimeofday (struct timeval *tv, struct timezone *tz);
