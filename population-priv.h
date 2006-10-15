#ifndef POPULATION_PRIV_H
#define POPULATION_PRIV_H		1
/*
 * population-priv.h -- Interface for the private side of a population.
 * $Id: population-priv.h,v 1.4 2002/06/04 16:33:57 rwx Exp $
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


struct population {
	size_t len;			/* Population size */
	struct individual **pop;	/* Array of individuals */
	struct individual *fittest;	/* Fittest individual */
	struct fitness_stats stats;	/* Fitness statistics */
	size_t mutations;		/* Number of mutations */
	size_t crossovers;		/* Number of crossovers */

	void *select_data;		/* Selector-specific data */
};


extern void population_compute_fitness_stats(struct population *self);

extern struct individual *population_get_fittest(struct population *self);

extern void population_print(struct population *self, FILE *fp);


#endif /* !POPULATION_PRIV_H */
