ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libeve.la
libeve_la_SOURCES = \
	chromosome.c \
	ga.c \
	individual.c \
	mt19937.c \
	operators.c \
	population.c \
	random.c \
	report.c

bin_PROGRAMS = test-ga
test_ga_SOURCES = test-ga.c
test_ga_LDFLAGS = -lm
test_ga_LDADD = libeve.la
