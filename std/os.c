/* zlib License
 *
 * Copyright (c) 2017-2025 Ed van Bruggen
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#include <limits.h>
#include <time.h>
#include <unistd.h>

/* TODO sys ls, mv, cp, rm, mkdir */

#define EEVO_SHORTHAND
#include "../eevo.h"

/* change to different directory */
Eevo
prim_cd(EevoSt st, EevoRec env, Eevo args)
{
	Eevo dir;
	eevo_arg_num(args, "cd!", 1);
	if (!(dir = eevo_eval(st, env, fst(args))))
		return NULL;
	eevo_arg_type(dir, "cd!", EEVO_TEXT);
	if (chdir(dir->v.s))
		return perror("; error: cd"), NULL;
	return &eevo_void;
}

/* return string of current working directory */
Eevo
prim_pwd(EevoSt st, EevoRec env, Eevo args)
{
	eevo_arg_num(args, "pwd", 0);
	char cwd[PATH_MAX];
	if (!getcwd(cwd, sizeof(cwd)) && cwd[0] != '(')
		eevo_warn("pwd: could not get current directory");
	return eevo_str(st, cwd);
}

/* exit program with return value of given int */
Eevo
prim_exit(EevoSt st, EevoRec env, Eevo args)
{
	Eevo code;
	eevo_arg_num(args, "exit!", 1);
	if (!(code = eevo_eval(st, env, fst(args))))
		return NULL;
	eevo_arg_type(code, "exit!", EEVO_INT);
	exit((int)code->v.n.num);
}

/* TODO time formating */
/* return number of seconds since 1970 (unix time stamp) */
Eevo
prim_now(EevoSt st, EevoRec env, Eevo args)
{
	eevo_arg_num(args, "now", 0);
	return eevo_int(st, time(NULL));
}

/* TODO time-avg: run timeit N times and take average */
/* return time in miliseconds taken to run command given */
Eevo
form_time(EevoSt st, EevoRec env, Eevo args)
{
	Eevo v;
	clock_t t;
	eevo_arg_num(args, "time", 1);
	t = clock();
	if (!(v = eevo_eval(st, env, fst(args))))
		return NULL;
	t = clock() - t;
	return eevo_dec(st, ((double)t)/CLOCKS_PER_SEC*100);
}
