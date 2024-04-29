#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ARRAY_SIZE 5

// Function to sort an array
void bubbleSort(int arr[], int n)
{
    int i, j, temp;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main()
{
    int arr[ARRAY_SIZE] = {12, 4, 7, 2, 9};
    int arr_sorted[ARRAY_SIZE];

    int fd1[2];
    int fd2[2];

    if (pipe(fd1) == -1 || pipe(fd2) == -1)
    {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        // Fork failed
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child process

        // Close unused ends of the pipes
        close(fd1[1]);
        close(fd2[0]);

        // Read the array from the pipe
        read(fd1[0], arr_sorted, sizeof(arr_sorted));

        // Sort the array
        bubbleSort(arr_sorted, ARRAY_SIZE);

        // Write the sorted array back to the pipe
        write(fd2[1], arr_sorted, sizeof(arr_sorted));

        // Close remaining ends of the pipes
        close(fd1[0]);
        close(fd2[1]);

        // Exit child process
        exit(EXIT_SUCCESS);
    }
    else
    {
        // Parent process

        // Close unused ends of the pipes
        close(fd1[0]);
        close(fd2[1]);

        // Write the array to the pipe
        write(fd1[1], arr, sizeof(arr));

        // Read the sorted array from the pipe
        read(fd2[0], arr_sorted, sizeof(arr_sorted));

        // Print the sorted array
        printf("Sorted array received from child process:\n");
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            printf("%d ", arr_sorted[i]);
        }
        printf("\n");

        // Close remaining ends of the pipes
        close(fd1[1]);
        close(fd2[0]);

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}
