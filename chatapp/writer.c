#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#define PIPE_NAME "mypipe"

int main()
{
    int fd;
    char input[256];
    // Open the named pipe for writing
    if (mkfifo(PIPE_NAME, 0777) == -1)
    {
        if (errno != EEXIST)
        {
            printf("Couldn't create named pipe\n");
            return 1;
        }
        // error cuz file already exist
    }
    fd = open(PIPE_NAME, O_WRONLY);
    if (fd == -1)
    {
        perror("Error opening pipe");
        exit(EXIT_FAILURE);
    }
    printf("Opening named pipe: %s\n", PIPE_NAME);
    while (1)
    {
        printf("Enter Input: ");
        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0'; // Remove newline character
        // Write input to the pipe
        ssize_t bytesWritten = write(fd, input, strlen(input));
        if (bytesWritten == -1)
        {
            perror("Error writing to pipe");
            exit(EXIT_FAILURE);
        }
        if (strcmp(input, "exit") == 0)
            break; // Exit when 'exit' is entered
    }
    printf("Writing buffer to pipe...done\n");
    printf("Exiting\n");
    close(fd);
    return 0;
}