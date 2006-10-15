/*
 * operators.c -- Implementation of several flavours of GA operators.
 * $Id: operators.c,v 1.3 2002/06/03 00:41:40 rwx Exp $
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
#endif /* STDC_HEADERS */

#include <assert.h>
#include <sys/types.h>

#include "random.h"
#include "chromosome.h"
#include "chromosome-priv.h"
#include "individual.h"
#include "individual-priv.h"
#include "population.h"
#include "population-priv.h"



struct topbottom_pairing_data {
	u_int current;
};

void
preselect_topbottom_pairing(struct population *pop)
{
	struct topbottom_pairing_data *data;

	assert(pop);	/* new_pop can be NULL */

	data = calloc(1, sizeof(struct topbottom_pairing_data));
	assert(data);	/* xmalloc */

	data->current = 0;

	assert(!pop->select_data);
	pop->select_data = data;
}

void
select_topbottom_pairing(struct population *pop,
			struct individual **dad, struct individual **mom)
{
	struct individual **pool;
	struct topbottom_pairing_data *data;

	assert(pop && mom && dad);

	data = (struct topbottom_pairing_data *) pop->select_data;

	pool = pop->pop;

	*dad = pool[data->current++];
	*mom = pool[data->current++];

	assert(*dad && *mom);
}



struct roulette_wheel_data {
	double total;
};

void
preselect_roulette_wheel(struct population *pop)
{
	struct roulette_wheel_data *data;

	assert(pop);

	data = calloc(1, sizeof(struct roulette_wheel_data));
	assert(data);

	data->total = population_get_fitness_stats(pop)->total;

	assert(!pop->select_data);
	pop->select_data = data;
}

static void
select_roulette_wheel_parent(struct population *pop, struct individual **parent)
{
	u_int i;
	double sum, pick = random_random();
	struct individual *cur_indiv;
	struct roulette_wheel_data *data;

	assert(pop && parent);

	data = (struct roulette_wheel_data *) pop->select_data;
	assert(data);

	for (i = 0, sum = 0.0, cur_indiv = NULL; (sum < pick) && (i < pop->len); i++) {
		cur_indiv = pop->pop[i];

		sum += individual_get_fitness(cur_indiv) / data->total;
	}

	*parent = cur_indiv;
}

void
select_roulette_wheel(struct population *pop,
			struct individual **dad, struct individual **mom)
{
	assert(pop && dad && mom);

	select_roulette_wheel_parent(pop, dad);
	select_roulette_wheel_parent(pop, mom);
}



struct tournament_data {
	struct individual **candidates;
};

void
preselect_tournament(struct population *pop)
{
	struct tournament_data *data;

	assert(pop);

	data = calloc(1, sizeof(struct tournament_data));
	assert(data);

}

void
select_tournament(struct population *pop,
			struct individual **dad, struct individual **mom)
{
	assert(pop && dad && mom);

}



void
crossover_single_point(struct individual *dad, struct individual *mom,
			struct individual **son, struct individual **daughter)
{
	u_int xsite;		/* Crossover point */
	size_t chrom_len;
	struct chromosome *cdad, *cmom, *cson, *cdaughter;

	assert(dad && mom && son && daughter);

	cdad = individual_get_chromosome(dad);
	cmom = individual_get_chromosome(mom);
	assert(cdad && cmom);

	chrom_len = chromosome_get_len(cdad);
	assert(chrom_len == chromosome_get_len(cdad));

	*son = new_individual(chrom_len);
	*daughter = new_individual(chrom_len);
	assert(*son && *daughter);

	cson = individual_get_chromosome(*son);
	cdaughter = individual_get_chromosome(*daughter);
	assert(cson && cdaughter);

	chromosome_copy(cdad, cson, 0, 0, chrom_len);
	chromosome_copy(cmom, cdaughter, 0, 0, chrom_len);

	xsite = random_range(0, chrom_len - 1);
	(*son)->xsite = (*daughter)->xsite = xsite;

	chromosome_copy(cmom, cson, xsite, xsite, chrom_len - xsite);
	chromosome_copy(cdad, cdaughter, xsite, xsite, chrom_len - xsite);
}

void
crossover_n_point(struct individual *dad, struct individual *mom,
			struct individual **son, struct individual **daughter)
{

}

void
crossover_uniform(struct individual *dad, struct individual *mom,
			struct individual **son, struct individual **daughter)
{

}



size_t
mutate(float pmutation, struct individual *indiv)
{
	u_int i;
	size_t mutations;
	struct chromosome *c;

	assert(indiv);

	c = individual_get_chromosome(indiv);
	assert(c);

	for (i = 0, mutations = 0; i < chromosome_get_len(c); i++) {
		if (random_flip(pmutation) == 0)
			continue;

		chromosome_not_allele(c, i);
		++mutations;
	}

	return mutations;
}
