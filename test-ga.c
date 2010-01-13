/*
 * test-ga.c -- Unit test for the genetic algorithm.
 * $Id: test-ga.c,v 1.3 2002/06/02 22:12:39 rwx Exp $
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

#include <stdint.h>

#include <time.h>
#include <math.h>
#include <assert.h>
#include <sys/types.h>

#include "random.h"
#include "chromosome.h"
#include "individual.h"
#include "population.h"
#include "ga.h"


static void test_ga(void);
static void objective(struct individual *i);
static double chromosome_to_double(struct chromosome *c);


int
main(int argc, char *argv[])
{
	test_ga();

	exit(EXIT_SUCCESS);
}


void
test_ga(void)
{
	struct ga *g;

	random_seed(time(NULL));

	g = new_ga(5000, 18, 100, 50, 0.8, 0.0001, 
			GA_S_TOPBOTTOM_PAIRING, GA_X_SINGLE_POINT, objective);
	assert(g);

	/* ga_set_report_strategy(g, GA_R_GRAPH); */
	ga_set_report_strategy(g, GA_R_HUMAN_READABLE);

	ga_evolve(g, 100);

	delete_ga(g);
}


void
objective(struct individual *i)
{
	double value;
	struct chromosome *c;

	assert(i);

	c = individual_get_chromosome(i);
	assert(c);

	value = chromosome_to_double(c);

	individual_set_fitness(i, value * value);
}


double
chromosome_to_double(struct chromosome *c)
{
	int i;
	uint32_t val;

	assert(c);

	for (i = 0, val = 0; i < chromosome_get_len(c); i++)
		if (chromosome_get_allele(c, i) == 1)
			val |= (1 << i);

	return (double) val;
}
