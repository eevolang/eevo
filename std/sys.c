/* zlib License
 *
 * Copyright (c) 2017-2026 Ed van Bruggen
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
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define EEVO_SHORTHAND
#include "../eevo.h"

/* Run system command with arguments */
Eevo
prim_sys(EevoSt st, EevoRec env, Eevo args)
{
	/* Count arguments */
	int argc = 0;
	for (Eevo cur = args; cur->t == EEVO_PAIR; cur = rst(cur))
		if (fst(cur)->t & EEVO_TEXT)
			argc++;
		/* TODO use print to convert value to string */
		else eevo_warn("sys: System command and arguments are not valid text");

	if (argc < 1)
		eevo_warn("sys: Missing system command to run");

	/* Copy argument list into array */
	char **argv = malloc((argc + 1) * sizeof(char *));
	if (!argv) return NULL;
	Eevo cur = args;
	for (int i = 0; i < argc; i++, cur = rst(cur)) {
		char *s = fst(cur)->v.s;
		int len = strlen(s) + 1;
		argv[i] = malloc(len * sizeof(char));
		strncpy(argv[i], s, len);
	}
	argv[argc] = NULL;
	char *argv0 = fst(args)->v.s;

	/* Fork and run command as child */
	int pid, status, err;
	switch (pid = fork()) {
	case -1:
		eevo_warn("sys: Could not fork to run command");
	case 0:
		execvp(argv0, argv);
		for (int i = 0; i < argc; i++)
			free(argv[i]);
		free(argv);
		return Nil;
	default:
		waitpid(pid, &status, 0);
		/* if child exited normally, return error code or void on success */
		if (WIFEXITED(status))
			return (err = WEXITSTATUS(status)) ? eevo_int(st, err) : Void;
		eevo_warnf("system: %s: Did not exit normally", argv0);
	}
}
