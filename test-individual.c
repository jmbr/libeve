/*
 * test-individual.c
 * $Id: test-individual.c,v 1.4 2002/06/02 22:12:20 rwx Exp $
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

#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif /* HAVE_LIMITS_H */

#include <assert.h>
#include <time.h>
#include <sys/types.h>

#include "chromosome.h"
#include "random.h"
#include "individual.h"
#include "individual-priv.h"


static void test_individual(void);
static void test_individual_random(void);
static void test_individual_dup(void);


int
main(int argc, char *argv[])
{
	test_individual();
	test_individual_random();
	test_individual_dup();

	exit(EXIT_SUCCESS);
}


void
test_individual(void)
{
	int j;
	struct individual *i;
	struct chromosome *c;
	size_t chrom_len = sizeof(u_int) * CHAR_BIT;
	double magick_fitness = 0.7777777;

	i = new_individual(chrom_len);
	assert(i);

	c = individual_get_chromosome(i);
	assert(c);

	for (j = 0; j < chrom_len; j++)
		assert(chromosome_get_allele(c, j) == 0);

	individual_set_fitness(i, magick_fitness);
	/* Dangerous double comparison */
	assert(individual_get_fitness(i) == magick_fitness);

	delete_individual(i);
}


void
test_individual_random(void)
{
	int j;
	size_t ones;
	struct individual *i;
	struct chromosome *c;
	size_t chrom_len = 100;

	random_seed(time(NULL), time(NULL) >> 8, time(NULL) >> 16);

	i = new_individual(chrom_len);
	assert(i);

	c = individual_get_chromosome(i);
	assert(c);

	individual_random(i);

	for (j = 0, ones = 0; j < chromosome_get_len(c); j++)
		if (chromosome_get_allele(c, j) == 0)
			++ones;

	/* True random chromosomes must have 50% approx. of 1s and 0s */
	assert(ones >= 40 && ones <= 60);

	delete_individual(i);
}


static int
chromosomes_are_identical(struct chromosome *lhs, struct chromosome *rhs)
{
	int status;
	char *sl, *sr;

	assert(lhs && rhs);

	/*
	 * This is a hack!
	 */

	sl = chromosome_as_string(lhs);
	sr = chromosome_as_string(rhs);

	status = strcmp(sl, sr);

	free(sl);
	free(sr);

	return ((status == 0) ? 1 : 0);
}

void
test_individual_dup(void)
{
	struct individual *i, *j;
	struct chromosome *k, *l;

	i = new_individual(24);
	assert(i);

	individual_random(i);

	j = individual_dup(i);
	assert(j);

	k = individual_get_chromosome(i);
	l = individual_get_chromosome(j);
	assert(k && l);

	assert(chromosomes_are_identical(k, l));

	delete_individual(i);
	delete_individual(j);
}
