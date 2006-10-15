/*
 * individual.c -- Deals with individuals.
 * $Id: individual.c,v 1.6 2002/06/02 22:12:20 rwx Exp $
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
#include "population.h"
#include "individual.h"
#include "individual-priv.h"


struct individual *
new_individual(size_t chrom_len)
{
	struct individual *i;

	i = calloc(1, sizeof(struct individual));
	if (!i)
		return NULL;

	i->chrom = new_chromosome(chrom_len);
	if (!i->chrom) {
		free(i);
		return NULL;
	}

	return i;
}

void
delete_individual(struct individual *self)
{
	assert(self);

	if (self->chrom) {
		/* See individual_dup for details on why this can happen. */
		delete_chromosome(self->chrom);
	}
	free(self);
}


struct chromosome *
individual_get_chromosome(struct individual *self)
{
	assert(self);

	return self->chrom;
}

void
individual_set_fitness(struct individual *self, double fitness)
{
	assert(self);

	self->fitness = fitness;
}

double
individual_get_fitness(struct individual *self)
{
	assert(self);

	return self->fitness;
}


void
individual_random(struct individual *self)
{
	u_int i;
	struct chromosome *c;
	void (*fn)(struct chromosome *, u_int);

	c = self->chrom;

	/* Slightly naive? Well... maybe */
	for (i = 0; i < chromosome_get_len(c); i++) {
		fn = (random_flip(0.5) == 1)
			? chromosome_set_allele
			: chromosome_clear_allele;

		fn(c, i);
	}
}


int
individual_compare(struct individual **lhs, struct individual **rhs)
{
	double f1, f2;
	int status = 0;

	f1 = individual_get_fitness(*lhs);
	f2 = individual_get_fitness(*rhs);

	if (f1 < f2)
		status = -1;
	else if (f1 == f2)
		status = 0;
	else if (f1 > f2)
		status = 1;

	return status;
}


struct individual *
individual_dup(struct individual *self)
{
	size_t chrom_len;
	struct individual *i;
	struct chromosome *c;

	assert(self);

	c = individual_get_chromosome(self);
	if (!c)
		return NULL;

	chrom_len = chromosome_get_len(c);

	i = new_individual(chrom_len);
	if (!i)
		return NULL;

	delete_chromosome(i->chrom);
	i->chrom = chromosome_dup(self->chrom);

	if (!i->chrom) {
		/*
		 * At this point we have an individual without a chromosome.
		 * That's the reason for the conditional in delete_individual.
		 */
		delete_individual(i);
		return NULL;
	}

	i->fitness = self->fitness;
		
	return i;
}


void
individual_print(struct individual *self, FILE *fp)
{
	char *s;

	assert(self && fp);

	s = chromosome_as_string(individual_get_chromosome(self));

	fprintf(fp, "%s (%.10f)\n", s, self->fitness);

	free(s);
}
