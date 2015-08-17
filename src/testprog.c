/*
//	@(#) testprog.c - exercises the three intercepted calls
//
//	License: Creative Commons CC0 
//		http://creativecommons.org/publicdomain/zero/1.0/legalcode
//	Author:	James Sainsbury
//		toves@sdf.lonestar.org
//
*/
# include	<sys/time.h>
# include	<time.h>
# include	<stdio.h>
# include	<stdlib.h>

main() {
	{
		time_t	t	= time (0);
		printf ("time(&t)\n");
		printf ("\t\tt\t= %ld\n\n", t);
	}

	{
		struct	timeval	tv	= {0, 0};
		gettimeofday (&tv, 0);
		printf ("gettimeofday(&tv, &tz)\n");
		printf ("\ttv.tv_sec\t= %ld\n", tv.tv_sec);
		printf ("\ttv.tv_usec\t= %ld\n\n", tv.tv_usec);
	}
	{
		struct	timespec	ts	= {0, 0};
		clock_gettime (CLOCK_REALTIME, &ts);
		printf ("clock_gettime(CLOCK_REALTIME, &ts)\n");
		printf ("\tts.tv_sec\t= %ld\n", ts.tv_sec);
		printf ("\tts.tv_nsec\t= %ld\n", ts.tv_nsec);
	}
	exit (EXIT_SUCCESS);
}
