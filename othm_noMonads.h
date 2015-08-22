#ifndef OTHM_NOMONADS_H
#define OTHM_NOMONADS_H

#include <othm_symbols.h>
#include <othm_base.h>
/* struct othm_nomonads_env { */
/* 	othm_hashmap *monads; */
/* }; */

extern char othm_noMonads_lambda_type[];
extern char othm_noMonads_chain_type[];
extern char othm_noMonads_prim_type[];
extern char othm_noMonads_comp_type[];

/* A general representation of computation in othm */
#define OTHM_CHAIN_FUNCT struct othm_pair (*) (struct othm_typed *, \
					       struct othm_typed *, \
					       struct othm_chain *)

struct othm_comp {
	struct othm_typed *type;
	void *meta_type;
	union {
		/* struct othm_lambda *lambda; */
		struct othm_symbol_struct *prim;
		struct othm_chain *chain;
	} group;
};

/* Lambda is can be used as a closure, but doesn't have to be */
/* struct othm_lambda { */
/* 	struct othm_hashmap *vars; */
/* 	struct othm_comp *function; */
/* }; */

/* struct othm_monad { */
/* 	void *type; */
/* 	/\* othm_comp *\/ */
/* 	struct othm_hashmap *bind; */
/* } */

struct othm_chain {
	struct othm_chain *next;
	struct othm_comp *here;
};


struct othm_comp *othm_comp_from_prim(struct othm_symbol_struct *prim);

struct othm_chain *othm_chain_direct(struct othm_comp *first, ...);

struct othm_typed *othm_chain_run(struct othm_hashmap *monad_map,
				  struct othm_chain *chain);

struct othm_pair othm_make_continuation(struct othm_typed *arg,
					struct othm_typed *state,
					struct othm_chain *position);

struct othm_pair othm_run_continuation(struct othm_typed *arg,
				       struct othm_typed *state,
				       struct othm_chain *position);


#endif
