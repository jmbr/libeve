/*
 * test-chromosome.c
 * $Id: test-chromosome.c,v 1.6 2002/06/04 01:06:56 rwx Exp $
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

#include <assert.h>
#include <sys/types.h>

#include "chromosome.h"
#include "chromosome-priv.h"


static void test_chromosome(void);
static void test_chromosome_dup(void);


int
main(int argc, char *argv[])
{
	test_chromosome();
	test_chromosome_dup();

	exit(EXIT_SUCCESS);
}


void
test_chromosome(void)
{
	int status;
	size_t len = 24;
	struct chromosome *c;
	char *s, orig[] = "100000000000000000000001";

	c = new_chromosome(len);
	assert(c);

	status = (len == chromosome_get_len(c));
	assert(status);

	chromosome_set_allele(c, 0);
	chromosome_set_allele(c, len / 2);
	chromosome_set_allele(c, len - 1);
	chromosome_not_allele(c, len / 2);

	s = chromosome_as_string(c);
	status = (strcmp(orig, s) == 0);
	free(s);

	assert(status);

	delete_chromosome(c);
}


void
test_chromosome_dup(void)
{
	int status;
	char *ss, *sd;
	size_t len = 24;
	struct chromosome *c, *d;

	c = new_chromosome(24);
	assert(c);

	chromosome_set_allele(c, 0);
	chromosome_set_allele(c, len - 1);

	d = chromosome_dup(c);
	assert(d);

	ss = chromosome_as_string(c);
	sd = chromosome_as_string(d);

	status = strcmp(ss, sd);
	assert(status == 0);

	free(ss);
	free(sd);
	delete_chromosome(c);
	delete_chromosome(d);
}
