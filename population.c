/*
 * population.c -- Implementation of population management code.
 * $Id: population.c,v 1.5 2002/06/04 16:33:57 rwx Exp $
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

#include <assert.h>
#include <sys/types.h>

#include "chromosome.h"
#include "individual.h"
#include "individual-priv.h"
#include "population.h"
#include "population-priv.h"


struct population *
new_population(size_t len)
{
	struct population *p;

	assert(len > 0);

	p = calloc(1, sizeof(struct population));
	if (!p)
		return NULL;

	p->len = len;
	p->pop = calloc(p->len, sizeof(struct individual *));
	if (!p->pop) {
		free(p);
		return NULL;
	}

	return p;
}

void
delete_population(struct population *self)
{
	size_t i;

	assert(self);

	if (self->pop)
		for (i = 0; i < self->len; i++)
			if (self->pop[i]) delete_individual(self->pop[i]);
	free(self->pop);
	free(self->select_data);
	free(self);
}


struct fitness_stats *
population_get_fitness_stats(struct population *self)
{
	assert(self);

	return &self->stats;
}


struct individual *
population_get_fittest(struct population *self)
{
	assert(self);

	qsort(self->pop, self->len, sizeof(struct individual *),
		(int (*)(const void *, const void *)) individual_compare);

	return self->pop[0];
}


void
population_compute_fitness_stats(struct population *self)
{
	int i;
	struct individual **pop;
	struct fitness_stats *stats;

	assert(self);

	stats = &self->stats;
	memset(stats, 0, sizeof(struct fitness_stats));

	pop = self->pop;

	/*
	 * This function assumes the population array has been sorted in
	 * advance (either by population_get_fittest or other means).
	 */
	stats->minimum = individual_get_fitness(pop[0]);
	stats->maximum = individual_get_fitness(pop[self->len - 1]);

	for (i = 0; i < self->len; i++)
		stats->total += individual_get_fitness(pop[i]);

	stats->average = stats->total / (double) self->len;
}


void
population_print(struct population *self, FILE *fp)
{
	int i;

        individual_print(population_get_fittest(self), stdout);
	/* for (i = 0; i < self->len; i++) */
	/* 	individual_print(self->pop[i], fp); */
}
