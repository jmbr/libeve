/*
 * evolution.c -- Implementation of the evolution mechanism.
 * $Id: evolution.c,v 1.1 2002/06/05 16:13:35 rwx Exp $
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

#include "individual.h"
#include "individual-priv.h"
#include "population.h"
#include "population-priv.h"
#include "ga.h"
#include "ga-priv.h"


static void ga_generation_epilogue(struct ga *self);


void
ga_evolve(struct ga *self, unsigned int max_gen)
{
	assert(self);
	
	self->current = 1;

	ga_first(self);
	while (self->current <= max_gen)
		ga_next(self);
}


void
ga_first(struct ga *self)
{
	unsigned int i;
	struct population *cur_pop;
	struct individual *cur_indiv;

	assert(self && !self->cur_pop);

	cur_pop = self->cur_pop = new_population(self->initial);

	/*
	 * Generate a random population.
	 */
	for (cur_indiv = NULL, i = 0; i < self->cur_pop->len; i++) {
		cur_indiv = cur_pop->pop[i] = new_individual(self->chrom_len);

		individual_random(cur_indiv);

		self->obj_fn(cur_indiv);
	}

	ga_generation_epilogue(self);
}


static void
swap_populations(struct population **old, struct population *new)
{
	if (*old)
		delete_population(*old);
	*old = new;
}

void
ga_next(struct ga *self)
{
	unsigned int i;
	struct population *cur_pop;
	struct individual *dad, *mom, *son, *daughter;

	assert(self && self->cur_pop);

	swap_populations(&self->old_pop, self->cur_pop);

	cur_pop = self->cur_pop = new_population(self->normal);

	ga_preselect(self);

	for (i = 0; i < self->normal; ) {
		self->select(self->old_pop, &dad, &mom);

		ga_cross(self, dad, mom, &son, &daughter);

		ga_mutate(self, son);
		ga_mutate(self, daughter);

		cur_pop->pop[i++] = son;
		cur_pop->pop[i++] = daughter;

		self->obj_fn(son);
		self->obj_fn(daughter);
	};

	ga_generation_epilogue(self);
}


void
ga_generation_epilogue(struct ga *self)
{
	assert(self);

	ga_set_best_ever(self, self->current,
			population_get_fittest(self->cur_pop));

	population_compute_fitness_stats(self->cur_pop);

	population_print(self->cur_pop, stdout);

	ga_report(self, stdout);

	++self->current;
}
