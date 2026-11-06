/*
Alunos:
Pedro Montarroyos de Pinho RA: 10440213
Gustavo Kiyoshi Ikeda RA: 10439179
Felipe Marques Leite Marta RA: 10437877

*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define DATA_SIZE 100
#define NUM_PROCS 5

int main(int argc, char *argv[])
{
    int process_rank, num_processes;
    int *data = NULL;        
    int *result = NULL;      
    int *local_data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

   
    if (num_processes != NUM_PROCS)
    {
        if (process_rank == 0)
        {
            fprintf(stderr, "Erro: execute com exatamente %d processos (ex.: mpirun -np %d ./mpi_quadrados)\n",
                    NUM_PROCS, NUM_PROCS);
        }
        MPI_Finalize();
        return 1;
    }

    int chunk_size = DATA_SIZE / num_processes; 

    
    if (process_rank == 0)
    {
        data = (int *)malloc(DATA_SIZE * sizeof(int));
        result = (int *)malloc(DATA_SIZE * sizeof(int));
        for (int i = 0; i < DATA_SIZE; i++)
        {
            data[i] = i + 1;
        }
    }

    
    local_data = (int *)malloc(chunk_size * sizeof(int));

    
    MPI_Scatter(data, chunk_size, MPI_INT,
                local_data, chunk_size, MPI_INT,
                0, MPI_COMM_WORLD);

    
    for (int i = 0; i < chunk_size; i++)
    {
        local_data[i] = local_data[i] * local_data[i];
    }

    
    MPI_Gather(local_data, chunk_size, MPI_INT,
               result, chunk_size, MPI_INT,
               0, MPI_COMM_WORLD);

    
    if (process_rank == 0)
    {
        printf("[Processo 0] Vetor original: [");
        for (int i = 0; i < DATA_SIZE; i++)
        {
            printf("%d%s", data[i], (i < DATA_SIZE - 1) ? ", " : "");
        }
        printf("]\n");

        printf("[Processo 0] Vetor transformado: [");
        for (int i = 0; i < DATA_SIZE; i++)
        {
            printf("%d%s", result[i], (i < DATA_SIZE - 1) ? ", " : "");
        }
        printf("]\n");
    }

    free(data);   
    free(result);
    free(local_data);

    MPI_Finalize();

    return 0;
}
