/*
 * report.c -- Implementation of the different reporting strategies available.
 * $Id: report.c,v 1.1 2002/06/02 22:03:03 rwx Exp $
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


static void report_human_readable(struct ga *self, FILE *fp);
static void report_graph(struct ga *self, FILE *fp);


void
ga_set_report_strategy(struct ga *self,
			enum ga_report_strategies report_strategy)
{
	report_fn report = NULL;

	assert(self);

	switch (report_strategy) {
	case GA_R_NOREPORT:
		break;
	case GA_R_HUMAN_READABLE:
		report = report_human_readable;
		break;
	case GA_R_GRAPH:
		report = report_graph;
		break;
	}

	self->report = report;
}


void
report_human_readable(struct ga *self, FILE *fp)
{
	struct fitness_stats *stats;

	stats = population_get_fitness_stats(self->cur_pop);
	assert(stats);

	fprintf(fp, ".----------------------------------------------------------------\n"); 
	fprintf(fp, "| Generation number: %3u / %3u\n", self->current, self->max_gen);
	fprintf(fp, "| Crossovers: %3u\tMutations: %3u\n", self->crossovers, self->mutations);
	fprintf(fp, "| Fittest individuals:\n");
	fprintf(fp, "|   so far (%3u):\t", self->best.generation);
	individual_print(self->best.i, fp);
	fprintf(fp, "|   in this generation:\t");
	individual_print(population_get_fittest(self->cur_pop), fp);
	fprintf(fp, "| Minimum fitness:\t%f\n", stats->minimum);
	fprintf(fp, "| Average fitness:\t%f\n", stats->average);
	fprintf(fp, "| Maximum fitness:\t%f\n", stats->maximum);
	fprintf(fp, "| Total fitness:\t%f\n", stats->total);
	fprintf(fp, "`----------------------------------------------------------------\n"); 
}

void
report_graph(struct ga *self, FILE *fp)
{
	struct fitness_stats *stats;

	stats = population_get_fitness_stats(self->cur_pop);
	assert(stats);

	fprintf(fp, "%u %f\n", self->current - 1, stats->average);
}
