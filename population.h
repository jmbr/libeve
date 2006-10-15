#ifndef POPULATION_H
#define POPULATION_H		1
/*
 * population.h -- Interface for population management code.
 * $Id: population.h,v 1.2 2002/06/01 12:09:28 rwx Exp $
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


/*
 * Statistics about fitness in a generation.
 */
struct fitness_stats {
	double total;		/* Total fitness of a population */
	double minimum;		/* Less fit individual's fitness */
	double average;		/* Average fitness per individual */
	double maximum;		/* Best individual's fitness */
};

struct population;


extern struct population *new_population(size_t len);
extern void delete_population(struct population *self);

extern struct fitness_stats *population_get_fitness_stats(struct population *self);


#endif /* !POPULATION_H */
