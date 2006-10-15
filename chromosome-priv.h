#ifndef CHROMOSOME_PRIV_H
#define CHROMOSOME_PRIV_H		1
/*
 * chromosome-priv.h -- Private side of a chromosome.
 * $Id: chromosome-priv.h,v 1.2 2002/06/01 12:13:06 rwx Exp $
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


struct chromosome {
	u_int *allele;		/* Genetic material */
	size_t len;		/* Length (in bits) */
};


extern struct chromosome *chromosome_dup(struct chromosome *self);

extern void chromosome_copy(struct chromosome *src, struct chromosome *dst,
				u_int src_pos, u_int dst_pos, size_t len);


#endif /* !CHROMOSOME_PRIV_H */
