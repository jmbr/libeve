#ifndef GA_H
#define GA_H		1
/*
 * ga.h -- Interface for the genetic algorithm.
 * $Id: ga.h,v 1.4 2002/06/02 22:01:13 rwx Exp $
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


struct ga;

/*
 * Records the best individual so far and remembers the generation where it
 * appeared.
 */
struct fittest {
	struct individual *i;
	unsigned generation;
};

enum ga_selection_strageties {
	GA_S_TOPBOTTOM_PAIRING
,	GA_S_ROULETTE_WHEEL
,	GA_S_TOURNAMENT
};

enum ga_crossover_strategies {
	GA_X_SINGLE_POINT,
	GA_X_N_POINT,
	GA_X_UNIFORM
};

enum ga_report_strategies {
	GA_R_NOREPORT = 0
,	GA_R_HUMAN_READABLE
,	GA_R_GRAPH		/* Output suitable for GNU plotutils' graph */
};

typedef void (*objective_fn)(struct individual *);


extern struct ga *new_ga(unsigned int max_gen,
			size_t chrom_len,
			size_t initial, size_t normal,
			float pcrossover, float pmutation,
			enum ga_selection_strategies selection_strategy,
			enum ga_crossover_strategies crossover_strategy,
			objective_fn obj_fn);
extern void delete_ga(struct ga *self);

extern void ga_set_report_strategy(struct ga *self, enum ga_report_strategies report_strategy);

extern void ga_first(struct ga *self);
extern void ga_next(struct ga *self);
extern void ga_evolve(struct ga *self, unsigned maxgen);

extern struct fittest *ga_get_best_ever(struct ga *self);


#endif /* !GA_H */
