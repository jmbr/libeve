#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H		1
/*
 * individual.h -- Deals with individuals.
 * $Id: individual.h,v 1.3 2002/05/30 17:04:18 rwx Exp $
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


struct individual;


extern struct individual *new_individual(size_t chrom_len);
extern void delete_individual(struct individual *self);

extern struct chromosome *individual_get_chromosome(struct individual *self);

extern void individual_set_fitness(struct individual *self, double fitness);
extern double individual_get_fitness(struct individual *self);


#endif /* !INDIVIDUAL_H */
