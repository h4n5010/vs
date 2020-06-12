#include "math.h"
#include <rpc/rpc.h>


int main(){

	printf("vor clnt create\n");
	struct CLIENT *c1 = clnt_create("localhost", MATHPROG, MATHVERS, "tcp");
	
	if(c1 < 0){
		perror("Error in clnt create!\n");
		exit(0);
	}
	printf("vor struct client\n");	

	intpair numbers;
	printf("vor number->a\n");
	numbers.a = 5;
	printf("vor number->b\n");
	numbers.b = 6;
	printf("nach numbers->b\n");
	int cube = 5;

	printf("vor add_1\n");
	int * result = add_1(&numbers, c1);
	printf("Addition: %d\n", *result);
	int * result1 = multiply_1(&numbers, c1);
	
	printf("Multiply: %d\n", *result1);
	int * result2 = cube_1(&cube, c1);

	printf("Cube: %d\n", *result2);
}
