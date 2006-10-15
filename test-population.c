/*
 * test-population.c
 * $Id: test-population.c,v 1.4 2002/06/04 01:06:17 rwx Exp $
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
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#endif /* STDC_HEADERS */

#include <time.h>
#include <assert.h>
#include <sys/types.h>

#include "random.h"
#include "chromosome.h"
#include "individual.h"
#include "population.h"
#include "population-priv.h"


static void test_population(void);


int
main(int argc, char *argv[])
{
	test_population();

	exit(EXIT_SUCCESS);
}


void
test_population(void)
{
	struct population *p;

	random_seed(time(NULL));

	p = new_population(32);
	assert(p);

	delete_population(p);
}
