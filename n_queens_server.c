#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "restart.h"
#include "uici.h"
#include "nqueens_library.h"
#define BUFFSIZE 10

int main(int argc, char *argv[])
{

    int bytescopied;
    pid_t child;
    char client[MAX_CANON];
    int communfd;
    int listenfd;
    u_port_t portnumber;

    char char_n[BUFFSIZE];
    char char_first[BUFFSIZE];
    int n;
    int first;
    int i;
    int result;

    if (argc != 2) // Check command line argument count
    {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        return 1;
    }

    portnumber = (u_port_t)atoi(argv[1]); // Convert command line argument to port number

    if ((listenfd = u_open(portnumber)) == -1) // Open the port to lisen on
    {
        perror("Failed to create listening endpoint");
        return 1;
    }

    fprintf(stderr, "[%ld]: Waiting for connection on port %d\n",
            (long)getpid(), (int)portnumber);

    while (TRUE)
    {
        // Try to accept the connection
        if ((communfd = u_accept(listenfd, client, MAX_CANON)) == -1)
        {
            perror("Failed to accept connection");
            continue;
        }

        switch (fork())
        {
        case -1:
            perror("Fork Error");
            exit(-1);
        case 0:
            // Close the listen file descriptor
            if (r_close(listenfd) == -1)
            {
                fprintf(stderr, "[%ld]:failed to close listenfd: %s\n",
                        (long)getpid(), strerror(errno));
                return 1;
            }
            // Get the arguments from client
            r_read(communfd, char_n, BUFFSIZE);
            r_read(communfd, char_first, BUFFSIZE);
            n = atoi(char_n);
            first = atoi(char_first);

            // fork and send solutions to client
            switch (fork())
            {
                case -1:
                    perror("Fork Error");
                    exit(-1);
                case 0:
                    result = generate_n_queens_serial_one(n, 0, first);
                    uint32_t netResult = htonl(result);

                    if(r_write(communfd, &netResult, sizeof(uint32_t)) == -1)
                    {
                        fprintf(stderr,"Error writing to client\n");
                    }
                    r_close(communfd);
                    fprintf(stderr, "[%ld]: Successful transfer to client.\n",
                                (long) getpid());
                    return 0;
            }
            return 0;
        default:
            while (r_waitpid(-1, NULL, WNOHANG) > 0);
        }
    }
}