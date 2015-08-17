#include <stdio.h>
#include "othm_noMonads.h"

struct othm_pair testing(void *a, void *b)
{
	printf("woo!");
	return OTHM_NULL_PAIR;
}

struct othm_pair testing2(void *a, void *b)
{
	if(a == NULL)
		printf("NULL WOOT!!!");
	return OTHM_NULL_PAIR;
}
OTHM_PRIM_FUNCT_INIT(testing, 1, struct othm_pair);
OTHM_PRIM_FUNCT_INIT(testing2, 2, struct othm_pair);

int main(void)
{
	struct othm_funct_chain *chain =
		othm_funct_chain_direct(othm_comp_from_prim
					(OTHM_PRIM_FUNCT(testing)),
					othm_comp_from_prim
					(OTHM_PRIM_FUNCT(testing2)),
					NULL);
	othm_run_chain(chain);
	return 0;
}
