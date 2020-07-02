#include <stdio.h>
#include <mpi.h>
#include <ctime>
#include <cstdlib>


void randArr(int *arr,int n)
{
        for (int i=0;i<n;i++)
                arr[i]=rand()%10+1;
}

int sumArr(int *arr,int n)
{
        int sum=0;
        for (int i=0;i<n;i++)
                sum+=arr[i];
        return sum;
}


void SRM(int rank,int size)
{
        int *arr,sum,n,buf;
        MPI_Status status;
        if (rank==0)
        {
        n=14;
        int part_size=n/size;
        int remain=n%size;
        arr=new int[n];
        randArr(arr,n);
        for (int i=0;i<n;i++)
                printf("%d ",arr[i]);
        printf("\n");
        MPI_Request request;
		for (int i=0;i<n;i++)
                printf("%d ",arr[i]);
        printf("\n");
        MPI_Request request;
        for (int i=1;i<size;i++)
        {
                MPI_Isend(arr+remain+part_size*i,part_size,MPI_INT,i,0,MPI_COMM_WORLD,&request);
                MPI_Request_free(&request);
        }
        sum=sumArr(arr,part_size+remain);
        for (int i=1;i<size;i++)
        {
                MPI_Recv(&buf,1,MPI_INT,i,0,MPI_COMM_WORLD,&status);
                sum+=buf;
        }
        printf("CPU=%d, sum=%d\n",rank,sum);
        }
        else
        {
        MPI_Probe(0,0,MPI_COMM_WORLD,&status);
        MPI_Get_count(&status,MPI_INT,&n);
        arr=new int[n];
        MPI_Recv(arr,n,MPI_INT,0,0,MPI_COMM_WORLD,&status);
        sum=sumArr(arr,n);
        MPI_Request request;
        MPI_Send(&sum,1,MPI_INT,0,0,MPI_COMM_WORLD);
        }
}


int main (int argc,char** argv)
{
        srand(time(NULL));
        int size;
        MPI_Init(NULL,NULL);
        int rank;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        SRM(rank,size);
        MPI_Finalize();
        return 0;
}

