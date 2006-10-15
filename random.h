#ifndef RANDOM_H
#define RANDOM_h        1
/*
 * random.h -- Wichman-Hill random number generator translated from Python.
 * $Id: random.h,v 1.3 2002/06/03 12:26:30 rwx Exp $
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


extern void random_seed(u_long seed);

extern double random_random(void);

extern int random_flip(float prob);

extern int random_range(int low, int high);


#endif /* !RANDOM_H */
