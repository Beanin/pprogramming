#include "mpimaster.h"
#include "mpiworker.h"

#include <mpi.h>


int main(int argc, char** argv)
{
	int WorkersCount, rank;
	if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
	{
		throw std::runtime_error("MPI_Init failed");
	}
	MPI_Comm_size(MPI_COMM_WORLD, &WorkersCount);
	WorkersCount--;
	if (WorkersCount < 1)
	{
		printf("1-node cluster. Turning off\n");
		MPI_Finalize();
		return 0;
		/*
		OmpMaster master(1);
		master.Id = 0;
		master.Handle();
		MPI_Finalize();
		return 0;
		*/
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
	{
		double start = MPI_Wtime();
		MpiMaster master(WorkersCount);
		master.Handle();
		double end = MPI_Wtime();
		printf("WTime:%f", end - start);	
	}
	else 
	{
		MpiWorker worker(rank, WorkersCount);
		worker.Handle();
	}
	MPI_Finalize();
}