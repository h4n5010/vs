#include "math.h"
#include <rpc/rpc.h>

int result_add;
int result_multiply;
int result_cube;


int * add_1_svc(intpair* pair,struct svc_req* rqstp){
	//intpair * pair;
	//struct svc_req *rqstp; /* not necessary in this example */
	
	result_add = (pair->a + pair->b);
	return &result_add;
}

int * multiply_1_svc(intpair* pair,struct svc_req* rqstp){
	//intpair * pair;
	//struct svc_req *rqstp; /* not necessary in this example */
	printf("Multiply1\n");
	result_multiply = (pair->a * pair->b);
	return &result_multiply;
}

int * cube_1_svc(int* pair,struct svc_req* rqstp){
	//intpair * pair;
	//struct svc_req *rqstp; /* not necessary in this example */
	printf("cube\n");
	result_cube = (*pair * *pair);
	return &result_cube;
}
