#include "math.h"
#include <rpc/rpc.h>

int result;

int * add_1_svc(intpair* pair,struct svc_req* rqstp){
	//intpair * pair;
	//struct svc_req *rqstp; /* not necessary in this example */
	result = (pair->a + pair->b);
	return &result;
}

int * multiply_1_svc(intpair* pair,struct svc_req* rqstp){
	//intpair * pair;
	//struct svc_req *rqstp; /* not necessary in this example */
	
	result = (pair->a * pair->b);
	return &result;
}

int * cube_1_svc(int* pair,struct svc_req* rqstp){
	//intpair * pair;
	//struct svc_req *rqstp; /* not necessary in this example */
	
	result = (*pair * *pair);
	return &result;
}
