#include "othm_noMonads.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


char othm_noMonads_lambda_type[] = "lambda";
char othm_noMonads_chain_type[] = "chain";
char othm_noMonads_prim_type[] = "prim-funct";
char othm_noMonads_comp_type[] = "comp";

struct othm_comp *othm_comp_from_prim(struct othm_symbol_struct *prim)
{
	struct othm_comp *new_comp;

	new_comp = malloc(sizeof(struct othm_comp));
	OTHM_TYPE(new_comp)->type = othm_noMonads_prim_type;
	new_comp->group.prim = prim;

	return new_comp;
}

struct othm_chain *othm_chain_direct(struct othm_comp *first, ...)
{
	va_list argp;
	struct othm_comp *comp;
	struct othm_chain *head;
	struct othm_chain *tail;

	if (first == NULL)
		return NULL;

	comp = first;
	head = malloc(sizeof(struct othm_chain));
	tail = head;
	va_start(argp, first);
	do {
		tail->here = comp;
	} while ((comp = va_arg(argp, struct othm_comp *)) ?
		 (tail->next = malloc(sizeof(struct othm_chain)),
		  tail = tail->next,
		  1): 0);
	/* This while is used to only allocate another part of the chain
	   if the list is not null! it does this using the comma operator
	   and the conditional operator */

	tail->next = NULL;
	va_end(argp);

	return head;
}

struct othm_typed *othm_chain_run(struct othm_hashmap *monad_map,
				  struct othm_chain *chain)
{
	struct othm_comp *comp;
	struct othm_chain *position;
	struct othm_pair pair;
	struct othm_typed *result = NULL;
	struct othm_typed *state = NULL;

	if(chain == NULL)
		return NULL;

	position = chain;
	do {
		pair = OTHM_PRIM_FUNCT_GET(position->here->group.prim,
					   OTHM_CHAIN_FUNCT)
			(result, state, position);
		result = pair.first;
		if (pair.second != NULL)
			position = (struct othm_chain *) pair.second;
		struct othm_comp *monad =
			othm_hashmap_get(monad_map,
					 othm_request_new
					 (othm_symbol_pointer_compare,
					  result->type,
					  sizeof(void *),
					  result->type));
		if(monad) {
			OTHM_PRIM_FUNCT_GET(monad->group.prim,
					    OTHM_CHAIN_FUNCT)
				(result, state, position);
		}
	} while(position = position->next);
	return result;
}

struct othm_pair othm_make_continuation(struct othm_typed *arg,
					struct othm_typed *state,
					struct othm_chain *position)
{
	return othm_pair_new(position, NULL);
}

struct othm_pair othm_run_continuation(struct othm_typed *arg,
				       struct othm_typed *state,
				       struct othm_chain *position)
{
	printf("woo!\n");
	return othm_pair_new(arg, arg);
}
