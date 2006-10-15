/*
 * chromosome.c -- Implementation of chromosome management.
 * $Id: chromosome.c,v 1.5 2002/06/02 22:03:55 rwx Exp $
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
# include <string.h>
#endif /* STDC_HEADERS */

#ifdef HAVE_LIMITS_H
# include <limits.h>
#endif /* HAVE_LIMITS_H */

#include <math.h>

#include <assert.h>
#include <sys/types.h>

#include "chromosome.h"
#include "chromosome-priv.h"


#ifndef UINTBITS
# define UINTBITS	(sizeof(u_int) * CHAR_BIT)
#endif /* !UINTBITS */

#define NMEMB(len)		((int) ceil((double) len / UINTBITS))


struct chromosome *
new_chromosome(size_t len)
{
	struct chromosome *c;

	if (!(c = malloc(sizeof(struct chromosome))))
		return NULL;

	if (!(c->allele = calloc(NMEMB(len), sizeof(int)))) {
		free(c);
		return NULL;
	}

	c->len = len;

	return c;
}

void
delete_chromosome(struct chromosome *self)
{
	assert(self);

	free(self->allele);
	free(self);
}


size_t
chromosome_get_len(struct chromosome *self)
{
	assert(self);

	return self->len;
}


static void
set(u_int *i, u_int pos)
{
	*i |= (u_int) (1 << pos);
}

static void
clear(u_int *i, size_t pos)
{
	*i &= (u_int) ~(1 << pos);
}

static u_int
get(const u_int *i, size_t pos)
{
	return (u_int) ((*i >> pos) & 1); 
}

#define LOCUS(pos)		((int) floor((double) pos / UINTBITS))
#define BITOP(op, allele, pos)	\
	(op(allele[LOCUS(pos)], pos % UINTBITS))

void
chromosome_set_allele(struct chromosome *self, u_int pos)
{
	assert(self && pos < self->len);

	BITOP(set, &self->allele, pos);
}

void
chromosome_clear_allele(struct chromosome *self, u_int pos)
{
	assert(self && pos < self->len);

	BITOP(clear, &self->allele, pos);
}

u_int
chromosome_get_allele(struct chromosome *self, u_int pos)
{
	assert(self && pos < self->len);

	return BITOP(get, &self->allele, pos);
}

#undef LOCUS
#undef BITOP

void
chromosome_not_allele(struct chromosome *self, u_int pos)
{
	assert(self && pos < self->len);

	switch (chromosome_get_allele(self, pos)) {
	case 0:
		chromosome_set_allele(self, pos);
		break;
	case 1:
		chromosome_clear_allele(self, pos);
		break;
	}
}


char *
chromosome_as_string(struct chromosome *self)
{
	u_int i;
	char *s;

	assert(self);

	s = calloc(1, self->len + 1);
	if (!s)
		return NULL;

	for (i = 0; i < self->len; i++)
		s[i] = (chromosome_get_allele(self, i) == 1) ? '1' : '0';

	return s;
}


struct chromosome *
chromosome_dup(struct chromosome *self)
{
	struct chromosome *c;

	assert(self);

	c = new_chromosome(self->len);
	if (!c)
		return NULL;

	memcpy(c->allele, self->allele, sizeof(u_int) * NMEMB(self->len));

	return c;
}


void
chromosome_copy(struct chromosome *src, struct chromosome *dst,
		u_int src_pos, u_int dst_pos, size_t len)
{
	u_int i;
	size_t chrom_len;

	assert(src && dst);

	chrom_len = chromosome_get_len(src);
	assert(chrom_len == chromosome_get_len(dst));

	assert(src_pos < chrom_len);
	assert(dst_pos < chrom_len);
	assert(src_pos + len <= chrom_len);
	assert(dst_pos + len <= chrom_len);

	for (i = 0; i < len; i++) {
		switch (chromosome_get_allele(src, src_pos + i)) {
		case 0:
			chromosome_clear_allele(dst, dst_pos + i);
			break;
		case 1:
			chromosome_set_allele(dst, dst_pos + i);
			break;
		default:
			assert(0);	/* Just in case something went wrong */
			break;
		}
	}
}
