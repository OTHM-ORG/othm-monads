#include "othm_noMonads.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


char othm_noMonads_lambda_type[] = "lambda";
char othm_noMonads_chain_type[] = "chain";
char othm_noMonads_prim_type[] = "prim-funct";

struct othm_comp *othm_comp_from_prim(struct othm_symbol_struct *prim)
{
	struct othm_comp *new_comp;

	new_comp = malloc(sizeof(struct othm_comp));
	OTHM_TYPE(new_comp)->type = othm_noMonads_prim_type;
	new_comp->group.prim = prim;

	return new_comp;
}

struct othm_funct_chain *othm_funct_chain_direct(struct othm_comp *first, ...)
{
	va_list argp;
	struct othm_comp *comp;
	struct othm_funct_chain *head;
	struct othm_funct_chain *tail;

	if (first == NULL)
		return NULL;

	comp = first;
	head = malloc(sizeof(struct othm_funct_chain));
	tail = head;
	va_start(argp, first);
	do {
		tail->here = comp;
	} while ((comp = va_arg(argp, struct othm_comp *)) ?
		 (tail->next = malloc(sizeof(struct othm_funct_chain)),
		  tail = tail->next,
		  1): 0);
	/* This while is used to only allocate another part of the chain
	   if the list is not null! it does this using the comma operator
	   and the conditional operator */

	tail->next = NULL;
	va_end(argp);

	return head;
}

void *othm_run_chain(struct othm_funct_chain *chain)
{
	struct othm_comp *comp;
	struct othm_funct_chain *position;
	struct othm_pair pair;
	void *result = NULL;
	void *state = NULL;

	if(chain == NULL)
		return NULL;

	position = chain;
	do {
		pair = OTHM_PRIM_FUNCT_GET(position->here->group.prim,
					   OTHM_CHAIN_FUNCT)
			(result, state);
		result = pair.first;
		state = pair.second;
	} while(position = position->next);
	return result;
}
