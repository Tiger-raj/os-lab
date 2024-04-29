#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define PIPE_NAME "mypipe"

int main()
{
    int fd;
    char buffer[256];
    // Create the named pipe
    mkfifo(PIPE_NAME, 0666);
    printf("Creating named pipe: %s\n", PIPE_NAME);
    printf("Waiting for input...\n");
    // Open the named pipe for reading
    fd = open(PIPE_NAME, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening pipe");
        exit(EXIT_FAILURE);
    }
    // Read strings from the pipe until 'exit' is received
    while (1)
    {
        ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
        if (bytesRead == -1)
        {
            perror("Error reading from pipe");
            exit(EXIT_FAILURE);
        }
        buffer[bytesRead] = '\0'; // Null-terminate the string
        printf("Got it: '%s'\n", buffer);
        if (strcmp(buffer, "exit") == 0)
            break; // Exit when 'exit' is received
    }
    printf("Exiting\n");
    close(fd);
    unlink(PIPE_NAME); // Remove the named pipe
    return 0;
}