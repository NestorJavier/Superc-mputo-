#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	int name, p;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &name);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	printf("Hola mundo, desde el proceso %d de %d\n",name,p);

	MPI_Finalize();
	
	return 0;
}
