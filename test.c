#include <stdio.h>
#include "othm_noMonads.h"

struct othm_typed TEST_TYPED = {
	.type = othm_noMonads_prim_type
};

struct othm_pair testing(struct othm_typed *arg,
			 struct othm_typed *state,
			 struct othm_chain *position)
{
	return othm_pair_new(&TEST_TYPED, NULL);
}

struct othm_pair testing2(struct othm_typed *arg,
			  struct othm_typed *state,
			  struct othm_chain *position)
{
	if(arg == NULL)
		printf("NULL WOOT!!!");
	return othm_pair_new(&TEST_TYPED, NULL);
}

struct othm_pair bordom_monad_prim(struct othm_typed *arg,
				   struct othm_typed *state,
				   struct othm_chain *position)
{
	printf("hello");
	position->next = NULL;
	return othm_pair_new(&TEST_TYPED, NULL);
}

OTHM_PRIM_FUNCT_INIT(testing, 1, struct othm_pair);
OTHM_PRIM_FUNCT_INIT(testing2, 2, struct othm_pair);
OTHM_PRIM_FUNCT_INIT(othm_make_continuation, 3, struct othm_pair);
OTHM_PRIM_FUNCT_INIT(othm_run_continuation, 4, struct othm_pair);
OTHM_PRIM_FUNCT_INIT(bordom_monad_prim, name, struct othm_pair);

int main(void)
{
	struct othm_chain *chain =
		othm_chain_direct(othm_comp_from_prim
				  (OTHM_PRIM_FUNCT(testing)),
				  othm_comp_from_prim
				  (OTHM_PRIM_FUNCT(testing2)),
				  othm_comp_from_prim
				  (OTHM_PRIM_FUNCT(othm_make_continuation)),
				  othm_comp_from_prim
				  (OTHM_PRIM_FUNCT(othm_run_continuation)),
				  NULL);

	struct othm_hashmap *monad_map = othm_hashmap_new();
	othm_hashmap_add(monad_map,
			 othm_request_new
			 (othm_symbol_pointer_compare,
			  TEST_TYPED.type,
			  sizeof(void *),
			  TEST_TYPED.type),
			 othm_comp_from_prim
			 (OTHM_PRIM_FUNCT(bordom_monad_prim)));
	struct othm_comp *monad =
		othm_hashmap_get(monad_map,
				 othm_request_new
				 (othm_symbol_pointer_compare,
				  TEST_TYPED.type,
				  sizeof(void *),
				  TEST_TYPED.type));
	printf("%s", (char *)
	       ((struct othm_funct *)
		(monad->group.prim)->request.data)
	       ->name);
	fflush(stdout);
	othm_chain_run(monad_map, chain);
	return 0;
}
