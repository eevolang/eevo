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
#include <assert.h>
#include <math.h>

/* wrapper functions to be returned by eevo_num, all need same arguments */
static Eevo
create_int(EevoSt st, double num, double den)
{
	assert(den == 1);
	return eevo_int(st, num);
}

static Eevo
create_dec(EevoSt st, double num, double den)
{
	assert(den == 1);
	return eevo_dec(st, num);
}

static Eevo
create_rat(EevoSt st, double num, double den)
{
	return eevo_rat(st, num, den);
}

/* return pointer to one of the preceding functions depending on what
 * number should be created by the following arithmetic functions
 * force arg is used to force number to one type:
 *   0 -> no force, 1 -> force ratio/int, 2 -> force decimal */
static Eevo
(*eevo_num(EevoType a, EevoType b, int force))(EevoSt, double, double)
{
	if (force == 1)
		return &create_rat;
	if (force == 2)
		return &create_dec;
	if (a & EEVO_DEC || b & EEVO_DEC)
		return &create_dec;
	if (a & EEVO_RATIO || b & EEVO_RATIO)
		return &create_rat;
	return &create_int;
}

#define PRIM_ROUND(NAME, FORCE)                                            \
Eevo                                                                       \
eevo_##NAME(EevoSt st, EevoRec env, Eevo args)                             \
{                                                                          \
	Eevo n;                                                            \
	eevo_arg_num(args, #NAME, 1);                                      \
	eevo_eval_args(st, env, args);                                     \
	n = fst(args);                                                     \
	eevo_arg_type(n, #NAME, EEVO_NUM);                                 \
	return (eevo_num(n->t, n->t, FORCE))(st, NAME(num(n)/den(n)), 1.); \
}

/* define int and dec as identity functions to use them in the same macro */
#define Int(X) (X)
#define Dec(X) (X)
PRIM_ROUND(Int,   1)
PRIM_ROUND(Dec,   2)
#undef Int
#undef Dec
PRIM_ROUND(round, 0)
PRIM_ROUND(floor, 0)
PRIM_ROUND(ceil,  0)

Eevo
eevo_add(EevoSt st, EevoRec env, Eevo args)
{
	Eevo a, b;
	eevo_arg_num(args, "+", 2);
	eevo_eval_args(st, env, args);
	a = fst(args), b = snd(args);
	eevo_arg_type(a, "+", EEVO_NUM);
	eevo_arg_type(b, "+", EEVO_NUM);
	if (a->t & EEVO_DEC || b->t & EEVO_DEC)
		return eevo_dec(st, (num(a)/den(a)) + (num(b)/den(b)));
	return (eevo_num(a->t, b->t, 0))
		(st, num(a) * den(b) + den(a) * num(b),
		     den(a) * den(b));
}

Eevo
eevo_sub(EevoSt st, EevoRec env, Eevo args)
{
	Eevo a, b;
	int len = eevo_lstlen(args);
	if (len != 2 && len != 1)
		eevo_warnf("-: expected 1 or 2 arguments, recieved %d", len);
	eevo_eval_args(st, env, args);
	a = fst(args);
	eevo_arg_type(a, "-", EEVO_NUM);
	if (len == 1) {
		b = a;
		a = eevo_int(st, 0);
	} else {
		b = snd(args);
		eevo_arg_type(b, "-", EEVO_NUM);
	}
	if (a->t & EEVO_DEC || b->t & EEVO_DEC)
		return eevo_dec(st, (num(a)/den(a)) - (num(b)/den(b)));
	return (eevo_num(a->t, b->t, 0))
		(st, num(a) * den(b) - den(a) * num(b),
		     den(a) * den(b));
}

Eevo
eevo_mul(EevoSt st, EevoRec env, Eevo args)
{
	Eevo a, b;
	eevo_arg_num(args, "*", 2);
	eevo_eval_args(st, env, args);
	a = fst(args), b = snd(args);
	eevo_arg_type(a, "*", EEVO_NUM);
	eevo_arg_type(b, "*", EEVO_NUM);
	if (a->t & EEVO_DEC || b->t & EEVO_DEC)
		return eevo_dec(st, (num(a)/den(a)) * (num(b)/den(b)));
	return (eevo_num(a->t, b->t, 0))(st, num(a) * num(b), den(a) * den(b));

}

Eevo
eevo_div(EevoSt st, EevoRec env, Eevo args)
{
	Eevo a, b;
	int len = eevo_lstlen(args);
	if (len != 2 && len != 1)
		eevo_warnf("/: expected 1 or 2 arguments, recieved %d", len);
	eevo_eval_args(st, env, args);
	a = fst(args);
	eevo_arg_type(a, "/", EEVO_NUM);
	if (len == 1) {
		b = a;
		a = eevo_int(st, 1);
	} else {
		b = snd(args);
		eevo_arg_type(b, "/", EEVO_NUM);
	}
	if (a->t & EEVO_DEC || b->t & EEVO_DEC)
		return eevo_dec(st, (num(a)/den(a)) / (num(b)/den(b)));
	return (eevo_num(a->t, b->t, 1))(st, num(a) * den(b), den(a) * num(b));
}

Eevo
eevo_mod(EevoSt st, EevoRec env, Eevo args)
{
	Eevo a, b;
	eevo_arg_num(args, "mod", 2);
	eevo_eval_args(st, env, args);
	a = fst(args), b = snd(args);
	eevo_arg_type(a, "mod", EEVO_INT);
	eevo_arg_type(b, "mod", EEVO_INT);
	if (num(b) == 0)
		eevo_warn("division by zero");
	return eevo_int(st, (int)num(a) % abs((int)num(b)));
}

/* TODO if given function as 2nd arg run it on first arg */
Eevo
eevo_pow(EevoSt st, EevoRec env, Eevo args)
{
	Eevo b, p;
	double bnum, bden;
	eevo_arg_num(args, "pow", 2);
	eevo_eval_args(st, env, args);
	b = fst(args), p = snd(args);
	eevo_arg_type(b, "pow", EEVO_EXPR);
	eevo_arg_type(p, "pow", EEVO_EXPR);
	bnum = pow(num(b), num(p)/den(p));
	bden = pow(den(b), num(p)/den(p));
	if ((bnum == (int)bnum && bden == (int)bden) ||
	     b->t & EEVO_DEC || p->t & EEVO_DEC)
		return eevo_num(b->t, p->t, 0)(st, bnum, bden);
	return eevo_list(st, 3, eevo_sym(st, "^"), b, p);
}

#define PRIM_COMPARE(NAME, OP)                      \
Eevo                                                \
eevo_##NAME(EevoSt st, EevoRec env, Eevo args)      \
{                                                   \
	if (eevo_lstlen(args) != 2)                 \
		return True;                        \
	eevo_eval_args(st, env, args);              \
	eevo_arg_type(fst(args), #OP, EEVO_NUM);    \
	eevo_arg_type(snd(args), #OP, EEVO_NUM);    \
	return ((num(fst(args))*den(snd(args)))  OP \
		(num(snd(args))*den(fst(args)))) ?  \
		True : Nil;                         \
}

PRIM_COMPARE(lt,  <)
PRIM_COMPARE(gt,  >)
PRIM_COMPARE(lte, <=)
PRIM_COMPARE(gte, >=)

#define PRIM_TRIG(NAME)                                          \
Eevo                                                             \
eevo_##NAME(EevoSt st, EevoRec env, Eevo args)                   \
{                                                                \
	eevo_arg_num(args, #NAME, 1);                            \
	eevo_eval_args(st, env, args);                           \
	eevo_arg_type(fst(args), #NAME, EEVO_EXPR);              \
	if (fst(args)->t & EEVO_DEC)                             \
		return eevo_dec(st, NAME(num(fst(args))));       \
	return eevo_list(st, 2, eevo_sym(st, #NAME), fst(args)); \
}

PRIM_TRIG(sin)
PRIM_TRIG(cos)
PRIM_TRIG(tan)
PRIM_TRIG(sinh)
PRIM_TRIG(cosh)
PRIM_TRIG(tanh)
PRIM_TRIG(asin)
PRIM_TRIG(acos)
PRIM_TRIG(atan)
PRIM_TRIG(asinh)
PRIM_TRIG(acosh)
PRIM_TRIG(atanh)
PRIM_TRIG(exp)
PRIM_TRIG(log)

Eevo
eevo_numerator(EevoSt st, EevoRec env, Eevo args)
{
	eevo_arg_num(args, "numerator", 1);
	eevo_eval_args(st, env, args);
	eevo_arg_type(fst(args), "numerator", EEVO_INT | EEVO_RATIO);
	return eevo_int(st, fst(args)->v.n.num);
}

Eevo
eevo_denominator(EevoSt st, EevoRec env, Eevo args)
{
	eevo_arg_num(args, "denominator", 1);
	eevo_eval_args(st, env, args);
	eevo_arg_type(fst(args), "denominator", EEVO_INT | EEVO_RATIO);
	return eevo_int(st, fst(args)->v.n.den);
}
