#ifndef OTHM_NOMONADS_H
#define OTHM_NOMONADS_H

#include <othm_symbols.h>
/* struct othm_nomonads_env { */
/* 	othm_hashmap *monads; */
/* }; */

/* A general representation of computation in othm */
#define OTHM_CHAIN_FUNCT struct othm_pair (*) (void *, void *)

struct othm_comp {
	struct othm_typed *type;
	union {
		struct othm_lambda *lambda;
		struct othm_symbol_struct *prim;
		struct othm_funct_chain *chain;
	} group;
};

/* Lambda is can be used as a closure, but doesn't have to be */
struct othm_lambda {
	struct othm_hashmap *vars;
	struct othm_comp *function;
};

struct othm_funct_chain {
	struct othm_funct_chain *next;
	struct othm_comp *here;
};

struct othm_comp *othm_comp_from_prim(struct othm_symbol_struct *prim);

struct othm_funct_chain *othm_funct_chain_direct(struct othm_comp *first, ...);

void *othm_run_chain(struct othm_funct_chain *chain);

#endif
