/*
 * ga.c -- Implementation of the genetic algorithm's front-end.
 * $Id: ga.c,v 1.3 2002/06/02 22:10:38 rwx Exp $
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
#include "individual.h"
#include "individual-priv.h"
#include "population.h"
#include "population-priv.h"
#include "ga.h"
#include "ga-priv.h"


static void ga_set_best_ever(struct ga *self, u_int generation,
				struct individual *candidate);

static void ga_set_selection_strategy(struct ga *self,
			enum ga_selection_strageties selection_strategy);

static void ga_set_crossover_strategy(struct ga *self,
			enum ga_crossover_strageties crossover_strategy);

static void ga_cross(struct ga *self, struct individual *dad,
			struct individual *mom, struct individual **son,
			struct individual **daughter);

static void ga_preselect(struct ga *self);

static void ga_mutate(struct ga *self, struct individual *indiv);

static void ga_report(struct ga *self, FILE *fp);


struct ga *
new_ga(u_int max_gen,
	size_t chrom_len,
	size_t initial, size_t normal,
	float pcrossover, float pmutation,
	enum ga_selection_strageties selection_strategy,
	enum ga_crossover_strageties crossover_strategy,
	objective_fn obj_fn)
{
	struct ga *g;

	assert(chrom_len > 0
		&& initial > 0 && normal > 0
		&& pcrossover > 0 && pmutation > 0
		&& obj_fn);

	if (initial < normal) {
		fprintf(stderr, "%s: The initial population must be at least "
			"as big as the normal population\n", __FUNCTION__);
		return NULL;
	}
	if ((initial % 2 != 0) || (normal % 2 != 0)) {
		fprintf(stderr, "%s: Populations must have an even number "
			"of individuals\n", __FUNCTION__);
		return NULL;
	}

	g = calloc(1, sizeof(struct ga));
	if (!g)
		return NULL;

	g->current = 1;
	g->max_gen = max_gen;

	g->chrom_len = chrom_len;

	g->initial = initial;
	g->normal = normal;

	g->pcrossover = pcrossover;
	g->pmutation = pmutation;

	g->crossovers = g->mutations = 0;

	ga_set_selection_strategy(g, selection_strategy);
	ga_set_crossover_strategy(g, crossover_strategy);
	g->mutate = mutate;

	g->old_pop = g->cur_pop = NULL;

	g->best.i = NULL;
	g->best.generation = 0;

	g->obj_fn = obj_fn;

	return g;
}

void
delete_ga(struct ga *self)
{
	assert(self);

	if (self->best.i)
		delete_individual(self->best.i);
	if (self->old_pop)
		delete_population(self->old_pop);
	if (self->cur_pop)
		delete_population(self->cur_pop);

	free(self);
}


void
ga_evolve(struct ga *self, u_int max_gen)
{
	assert(self);

	ga_first(self);
	while (self->current <= self->max_gen)
		ga_next(self);
}


void
ga_first(struct ga *self)
{
	u_int i;
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

	ga_set_best_ever(self, self->current,
			population_get_fittest(cur_pop));

	population_compute_fitness_stats(cur_pop);

	ga_report(self, stdout);

	++self->current;
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
	u_int i;
	struct population *cur_pop;
	struct individual *dad, *mom, *son, *daughter;	/* YHVH */

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

	ga_set_best_ever(self, self->current,
			population_get_fittest(cur_pop));

	population_compute_fitness_stats(cur_pop);

	ga_report(self, stdout);

	++self->current;
}


void
ga_preselect(struct ga *self)
{
	if (self->preselect)
		self->preselect(self->old_pop);
}

void
ga_cross(struct ga *self, struct individual *dad, struct individual *mom,
	struct individual **son, struct individual **daughter)
{
	if (random_flip(self->pcrossover) == 1) {
		self->cross(dad, mom, son, daughter);
		++self->crossovers;
	} else {
		*son = individual_dup(dad);
		*daughter = individual_dup(mom);
	}
}

void
ga_mutate(struct ga *self, struct individual *indiv)
{
	self->mutations += self->mutate(self->pmutation, indiv);
}


void
ga_set_best_ever(struct ga *self, u_int generation,
		struct individual *candidate)
{
	int status, should_change = 1;
	struct individual *best;

	assert(self && candidate);

	best = self->best.i;
	if (best) {
		status = individual_compare(&best, &candidate);
		should_change = (status == 1) ? 1 : 0;
	}

	/*
	 * In case the candidate is actually better than the best individual so
	 * far (or in case there's no best yet)...
	 */
	if (should_change) {
		if (best)
			delete_individual(best);

		self->best.i = individual_dup(candidate);
		self->best.generation = generation;
	}
}

struct fittest *
ga_get_best_ever(struct ga *self)
{
	assert(self);

	return &self->best;
}


void
ga_set_selection_strategy(struct ga *self,
			enum ga_selection_strageties selection_strategy)
{
	preselection_fn preselect = NULL;
	selection_fn select = NULL;

	assert(self);

	switch (selection_strategy) {
	case GA_S_TOPBOTTOM_PAIRING:
		preselect = preselect_topbottom_pairing;
		select = select_topbottom_pairing;
		break;
	case GA_S_ROULETTE_WHEEL:
		preselect = preselect_roulette_wheel;
		select = select_roulette_wheel;
		break;
	case GA_S_TOURNAMENT:
		preselect = preselect_tournament;
		select = select_tournament;
		break;
	}

	self->preselect = preselect;
	self->select = select;
}

void
ga_set_crossover_strategy(struct ga *self,
			enum ga_crossover_strageties crossover_strategy)
{
	crossover_fn cross = NULL;

	assert(self);

	switch (crossover_strategy) {
	case GA_X_SINGLE_POINT:
		cross = crossover_single_point;
		break;
	case GA_X_N_POINT:
		cross = crossover_n_point;
		break;
	case GA_X_UNIFORM:
		cross = crossover_uniform;
		break;
	}

	self->cross = cross;
}


void
ga_report(struct ga *self, FILE *fp)
{
	assert(self && fp);

	if (self->report)
		self->report(self, fp);
}
