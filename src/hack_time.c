/*
//	@(#) hack_time.c
//
//	License: Creative Commons CC0 
//		http://creativecommons.org/publicdomain/zero/1.0/legalcode
//	Author:	James Sainsbury
//		toves@sdf.org
//
*/
# define	_GNU_SOURCE
# include	<dlfcn.h>

# include	<sys/time.h>
# include	<time.h>
# include	<errno.h>
# include	<stdlib.h>

# include	"constant.h"

extern	long	__attribute__((visibility("hidden")))	clock_shift;

static	time_t	(*original_time)(time_t*);

static	void	__attribute__((constructor))	hack_time_init() {
	original_time		= dlsym (RTLD_NEXT, "time");
}

time_t	__attribute__((visibility("hidden"))) intercept_time (time_t* t) {
	time_t	result	= (time_t)(-1);
	if (original_time) {
		result	= original_time (0);
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

time_t	__attribute__((alias("intercept_time"))) time (time_t* t);
