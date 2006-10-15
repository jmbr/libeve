/*
 * random.c -- Wichman-Hill random number generator translated from Python.
 * $Id: random.c,v 1.4 2002/06/03 12:26:30 rwx Exp $
 */
/*
 * Copyright (C) 2002 Juan M. Bello Rivas <rwx@synnergy.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include "config.h"

#ifdef STDC_HEADERS
# include <stdlib.h>
#endif /* STDC_HEADERS */

#include <time.h>
#include <math.h>
#include <sys/types.h>

#include "random.h"


extern void sgenrand(unsigned long seed);
extern double genrand(void);


void
random_seed(u_long seed)
{
	sgenrand(seed);
}


double
random_random(void)
{
	return genrand();
}


int
random_flip(float prob)
{
	return (random_random() <= prob) ? 1 : 0;
}


int
random_range(int low, int high)
{
	int i;

	if (low >= high)
		i = low;
	else {
		i = (random_random() * (high - low + 1)) + low;
		if (i > high)
			i = high;
	}

	return i;
}
