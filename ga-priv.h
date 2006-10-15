#ifndef GA_PRIV_H
#define GA_PRIV_H		1
/*
 * ga-priv.h -- Private interface for the genetic algorithm.
 * $Id: ga-priv.h,v 1.2 2002/06/02 22:08:37 rwx Exp $
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


typedef void (*preselection_fn)(struct population *);

typedef void (*selection_fn)(struct population *,
				struct individual **, struct individual **);

typedef void (*crossover_fn)(struct individual *, struct individual *,
				struct individual **, struct individual **);

typedef size_t (*mutation_fn)(float, struct individual *);

typedef void (*report_fn)(struct ga *, FILE *);

/*
 * A genetic algorithm.
 */
struct ga {
	unsigned int current;		/* Current generation index */
	unsigned int max_gen;		/* Maximum generation number */

	size_t chrom_len;		/* Length of a chromosome */

	size_t initial;			/* Individuals at 1st generation */
	size_t normal;			/* Individuals in a generation */

	float pcrossover;		/* Probability of crossover */
	float pmutation;		/* Probability of mutation */
	
	size_t crossovers;		/* Ammount of crossovers */
	size_t mutations;		/* Ammount of mutations */

	struct population *old_pop;	/* Previous population */
	struct population *cur_pop;	/* Current population */

	struct fittest best;		/* Best individual so far */

	objective_fn obj_fn;		/* User-defined objective function */

	preselection_fn preselect;	/* Preselection operator */
	selection_fn select;		/* Selection operator */

	crossover_fn cross;		/* Crossover operator */

	mutation_fn mutate;		/* Mutation operator */

	report_fn report;		/* Concrete report strategy */
};


extern void preselect_topbottom_pairing(struct population *pop);
extern void select_topbottom_pairing(struct population *pop,
					struct individual **dad,
					struct individual **mom);

extern void preselect_roulette_wheel(struct population *pop);
extern void select_roulette_wheel(struct population *pop,
					struct individual **dad,
					struct individual **mom);

extern void preselect_tournament(struct population *pop);
extern void select_tournament(struct population *pop,
					struct individual **dad,
					struct individual **mom);


extern void crossover_single_point(struct individual *dad,
					struct individual *mom,
					struct individual **son,
					struct individual **daughter);
extern void crossover_n_point(struct individual *dad, struct individual *mom,
				struct individual **son,
				struct individual **daughter);
extern void crossover_uniform(struct individual *dad, struct individual *mom,
				struct individual **son,
				struct individual **daughter);


extern size_t mutate(float pmutation, struct individual *indiv);


#endif /* !GA_PRIV_H */
