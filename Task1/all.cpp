#include <stdio.h>
#include <mpi.h>

void SRM(int **arr,int rank,int size)
{
        int send=rank,recv;
        int last=0;
        printf("Before send:buf=%d in CPU=%d\n",send,rank);
        MPI_Request request;
        MPI_Status status;
        for (int i=0;i<size;i++)
        {
        if (i!=rank)
                {
                MPI_Isend(&send,1,MPI_INT,i,0,MPI_COMM_WORLD,&request);
                MPI_Request_free(&request);
                MPI_Recv(&recv,1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,&status);
                arr[rank][last]=recv;
                ++last;
                }
        }
}


int main (int argc,char** argv)
{
        int size;
        int n=8;
        int **arr=new int*[n];
        for (int i=0;i<n;i++)
                arr[i]=new int[n-1];

        MPI_Init(NULL,NULL);
        int rank;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        SRM(arr,rank,size);
        MPI_Finalize();
        printf("CPU=%d\n",rank);
        for (int j=0;j<size-1;j++)
        printf("%d ",arr[rank][j]);
        printf("\n");
        return 0;
}
