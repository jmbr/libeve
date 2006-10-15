#ifndef CHROMOSOME_H
#define CHROMOSOME_H		1
/*
 * chromosome.h -- Interface for chromosome management.
 * $Id: chromosome.h,v 1.3 2002/06/02 22:03:55 rwx Exp $
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


struct chromosome;


extern struct chromosome *new_chromosome(size_t len);
extern void delete_chromosome(struct chromosome *self);

extern size_t chromosome_get_len(struct chromosome *self);

extern void chromosome_set_allele(struct chromosome *self, unsigned int pos);
extern void chromosome_clear_allele(struct chromosome *self, unsigned int pos);
extern void chromosome_not_allele(struct chromosome *self, unsigned int pos);
extern unsigned int chromosome_get_allele(struct chromosome *self, unsigned int pos);

extern char *chromosome_as_string(struct chromosome *self);


#endif /* !CHROMOSOME_H */
