#include "math.h"
#include <rpc/rpc.h>


int main(){


	clnt_create("localhost", MATHPROG, MATHVERS, "tcp");

	struct CLIENT *c1;	

	intpair * numbers;
	numbers->a = 5;
	numbers->b = 6;

	int * cube = 5;

	int * result = add_1(numbers, c1);
	printf("Addition: %d\n", *result);
	int * result1 = multiply_1(numbers, c1);
	
	printf("Multiply: %d\n", *result);
	int * result2 = cube_1(cube, c1);

	printf("Cube: %d\n", *result);
}


