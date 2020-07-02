#include <stdio.h>
#include <mpi.h>

void SRM(int rank,int size)
{
        int send=rank,recv;
        printf("Before send:buf=%d in CPU=%d\n",send,rank);

        MPI_Status status;
        MPI_Sendrecv(&send,1,MPI_INT,(rank+1)%size,0,&recv,1,MPI_INT,(rank-1+size)%size,0,MPI_COMM_WORLD,&status);
        printf("After send and recieve: buf=%d in CPU=%d\n",recv,rank);
}


int main (int argc,char** argv)
{
        int size;
        MPI_Init(NULL,NULL);
        int rank;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        SRM(rank,size);
        MPI_Finalize();
        return 0;
}

