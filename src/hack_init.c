/*
//	@(#) hack_init.c
//
//	License: Creative Commons CC0 
//		http://creativecommons.org/publicdomain/zero/1.0/legalcode
//	Author:	James Sainsbury
//		toves@sdf.org
//
*/
# include	<sys/time.h>
# include	<time.h>
# include	<errno.h>
# include	<stdlib.h>


static	char	HACK_CLOCK_SHIFT[]	= "HACK_CLOCK_SHIFT";

long	__attribute__((visibility("hidden"))) clock_shift	= 0;

void	__attribute__((constructor, visibility("hidden"))) hack_clock_init() {

	char*	env	= getenv (HACK_CLOCK_SHIFT);
	if (env) {
		clock_shift		= strtol (env, 0, 0);
	}
}
