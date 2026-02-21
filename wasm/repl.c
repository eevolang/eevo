/* See LICENSE file for copyright and license details. */
#include <libgen.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../eevo0.2.h"

/* read, parse, and eval file given as single element list, or empty list for stdin */
static Eevo
parse_eval(EevoSt st, char *input)
{
	Eevo read = eevo_str(st, input);
	Eevo val = eevo_list(st, 2, eevo_sym(st, "parse"), read);
	val = eevo_eval(st, st->env, val); /* read and parse */
	return eevo_eval(st, st->env, val); /* eval resulting expressions */
}

/* if lib 2nd arg != NULL parse, eval, print it */
/* TODO: move to eevo.c, merge with eevo_env_lib, or replace w/ simple main example? */
char *
eevo_read_eval_print(char *input)
{
	static EevoSt st = NULL;
	if (!st) { /* Initialize on first use */
		st = eevo_env_init(1024);
		eevo_env_lib(st);
	}

	Eevo v = NULL;
	char *ret = NULL;

	v = parse_eval(st, input);
	/* TODO: use display, and write to stdout */
	if (v && v->t != EEVO_VOID)
		ret = eevo_print(v);

	return ret;
}
